#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "memlib.h"
#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Ranger",
    /* First member's full name */
    "DuGuosheng",
    /* First member's email address */
    "duguosheng99@foxmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/*
 * If NEXT_FIT defined use next fit search, else use first-fit search
 */
#define NEXT_FIT

/* Basic constants and macros */
#define WSIZE       4            // 字长以及头部脚部的长度
#define DSIZE       8            // 双字长
#define CHUNKSIZE  (1 << 12)     // 扩展堆时的默认字节数

#define MAX(x, y) ((x) > (y) ? (x) : (y))

// 将大小和已分配位结合起来，用于放在头部和脚部中
// 头部的格式为31~3位描述大小，2~0位描述是否分配(000未分配 001已分配)
#define PACK(size, alloc) ((size) | (alloc))

// 在地址p处读写一个字
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// 从地址p处获取分配大小和分配情况
// 头部的格式为31~3位描述大小，2~0位描述是否分配(000未分配 001已分配)
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) &  0x1)

// 给定一个块指针bp，计算它的头部和脚部的地址
// 注意，bp指向第一个有效载荷字节
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

// 计算前一个块的脚部地址
#define PREV_FTRP(bp)    ((char *)(bp) - DSIZE)

// 给定一个块指针bp，计算前后一个块的地址
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(PREV_FTRP(bp)))

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */
#ifdef NEXT_FIT
static char *rover;           /* Next fit rover */
#endif

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp);
static void checkheap(int verbose);
static void checkblock(void *bp);

/*
 * mm_init - 初始化内存管理
 */
int mm_init(void)
{
    // 创建初始空堆
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                             // 对齐填充
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  // 序言块的头部
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  // 序言块的脚部
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      // 第一个普通块的头部
    heap_listp += (2 * WSIZE);                      // 堆链表指向序言块的下一个小块，这样HDRP等操作就统一了

#ifdef NEXT_FIT
    rover = heap_listp;
#endif

    // 用一个CHUNKSIZE字节大小的空闲块扩展空堆
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}

/*
 * mm_malloc - Allocate a block with at least size bytes of payload
 */
void *mm_malloc(size_t size)
{
    size_t asize;      /* Adjusted block size */
    size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;

    if (heap_listp == 0) {
        mm_init();
    }
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    // 最小分配字节数为16，8字节满足对齐，8字节存放头部脚部
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    return bp;
}

/*
 * mm_free - Free a block
 */
void mm_free(void *bp)
{
    if (bp == 0)
        return;

    // 读取分配块的大小
    size_t size = GET_SIZE(HDRP(bp));
    if (heap_listp == 0) {
        mm_init();
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    // 合并空闲块
    coalesce(bp);
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    // 前后都已经分配，则不合并
    if (prev_alloc && next_alloc) {            /* Case 1 */
        return bp;
    }

    // 前面已分配而后面未分配
    else if (prev_alloc && !next_alloc) {      /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    // 后面已分配而前面未分配
    else if (!prev_alloc && next_alloc) {      /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    // 前后均未分配
    else {                                     /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
#ifdef NEXT_FIT
    /* Make sure the rover isn't pointing into the free block */
    /* that we just coalesced */
    if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp)))
        rover = bp;
#endif
    return bp;
}

/*
 * mm_realloc - Naive implementation of realloc
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t oldsize;
    void *newptr;

    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0) {
        mm_free(ptr);
        return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if (ptr == NULL) {
        return mm_malloc(size);
    }

    newptr = mm_malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if (!newptr) {
        return 0;
    }

    /* Copy the old data. */
    oldsize = GET_SIZE(HDRP(ptr));
    if (size < oldsize) oldsize = size;
    memcpy(newptr, ptr, oldsize);

    /* Free the old block. */
    mm_free(ptr);

    return newptr;
}

/*
 * mm_checkheap - Check the heap for correctness
 */
void mm_checkheap(int verbose)
{
    checkheap(verbose);
}

/*
 * The remaining routines are internal helper routines
 */

/*
 * extend_heap - 使用空闲块扩展堆，并返回块指针
 * 此函数在两种情况下使用：初始化堆; mm_malloc无法找到合适的匹配块
 */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    // 分配偶数个字以保持对齐
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    // mem_sbrk返回此前的brk指针，即新分配空间的起始
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* 新增空闲的头部 */  
    PUT(FTRP(bp), PACK(size, 0));         /* 新增空闲的脚部 */  
    // 结束块是一个大小为0的已分配块
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* 新的结束块 */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

/*
 * place - Place block of asize bytes at start of free block bp
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));

    if ((csize - asize) >= (2 * DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/*
 * find_fit - Find a fit for a block with asize bytes
 */
static void *find_fit(size_t asize)
{

#ifdef NEXT_FIT 
    /* Next fit search */
    char *oldrover = rover;

    /* Search from the rover to the end of list */
    for (; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    /* search from start of list to old rover */
    for (rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover))
        if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
            return rover;

    return NULL;  /* no fit found */
#else 
    /* First-fit search */
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
            return bp;
        }
    }
    return NULL; /* No fit */
#endif
}

static void printblock(void *bp)
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));

    if (hsize == 0) {
        printf("%p: EOL\n", bp);
        return;
    }

    printf("%p: header: [%ld:%c] footer: [%ld:%c]\n", bp,
        hsize, (halloc ? 'a' : 'f'),
        fsize, (falloc ? 'a' : 'f'));
}

static void checkblock(void *bp)
{
    if ((size_t)bp % 8)
        printf("Error: %p is not doubleword aligned\n", bp);
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
        printf("Error: header does not match footer\n");
}

/*
 * checkheap - Minimal check of the heap for consistency
 */
void checkheap(int verbose)
{
    char *bp = heap_listp;

    if (verbose)
        printf("Heap (%p):\n", heap_listp);

    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
        printf("Bad prologue header\n");
    checkblock(heap_listp);

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (verbose)
            printblock(bp);
        checkblock(bp);
    }

    if (verbose)
        printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
        printf("Bad epilogue header\n");
}

