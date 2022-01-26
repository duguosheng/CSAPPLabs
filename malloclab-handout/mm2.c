/*
 * mm2.c - 显式空闲链表分配
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

// 给定一个块指针bp，计算它的头部的地址
// 注意，bp指向第一个有效载荷字节
#define HDRP(bp)        ((char *)(bp) - 3 * WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - (3 * DSIZE))

// 给定一个块指针bp，计算前后一个块的地址
#define PREV_BLKP(bp) (*(char **)((char *)(bp) - DSIZE))
#define NEXT_BLKP(bp) (*(char **)((char *)(bp) - WSIZE))

static void *extend_heap(size_t words);
static void *coalesce(void *bp);

static char *heap_listp = NULL;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    // 创建初始空堆
    if ((heap_listp=mem_sbrk(4 * DSIZE)) == (void*)-1) {
        return -1;
    }

    // 有效载荷双字对齐
    PUT(heap_listp, 0);                             // 对齐
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 0));  // header
    PUT(heap_listp + (2 * WSIZE), 0);               // prev
    PUT(heap_listp + (3 * WSIZE),                   // succ
        (unsigned int)(heap_listp + (2 * DSIZE)));
    PUT(heap_listp + (3 * WSIZE), PACK(DSIZE, 1));  // footer
       
    heap_listp += 2 * DSIZE;

    PUT(heap_listp, PACK(DSIZE, 0));    // header
    PUT(heap_listp + (1 * WSIZE), );    // prev
    PUT(heap_listp + (2 * WSIZE),       // succ
        (unsigned int)(heap_listp + (2 * DSIZE)));
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

    return NULL;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    return NULL;
}

int mm_check(void)
{
    return 0;
}

static void *extend_heap(size_t words) {

}

static void *coalesce(void *bp) {

}
