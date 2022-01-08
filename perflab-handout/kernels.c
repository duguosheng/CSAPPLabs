/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Please fill in the following team struct
 */
team_t team = {
    "Ranger", /* Team name */

    "DuGuosheng", /* First member full name */
    "duguosheng99@foxmail.com", /* First member email address */

    "", /* Second member full name (leave blank if none) */
    "" /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/
char rotate_v1_descr[] = "rotate_v1: 将dst按行展开，从而省去了频繁的dim-1-j计算";
void rotate_v1(int dim, pixel* src, pixel* dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = src[RIDX(j, dim - 1 - i, dim)];
}

void rotate_block(int dim, pixel* src, pixel* dst, int block)
{
    int i, j, x, y;

    for (i = 0; i < dim; i += block)
        for (j = 0; j < dim; j += block)
            for (x = i; x < i + block; ++x)
                for (y = j; y < j + block; ++y)
                    dst[RIDX(x, y, dim)] = src[RIDX(y, dim - 1 - x, dim)];
}

char rotate_v2_descr[] = "rotate_v2: 将矩阵分为8X8的块";
void rotate_v2(int dim, pixel* src, pixel* dst)
{
    rotate_block(dim, src, dst, 8);
}

char rotate_v3_descr[] = "rotate_v3: 将矩阵分为16X16的块";
void rotate_v3(int dim, pixel* src, pixel* dst)
{
    rotate_block(dim, src, dst, 16);
}

char rotate_v4_descr[] = "rotate_v4: 将矩阵分为32X32的块";
void rotate_v4(int dim, pixel* src, pixel* dst)
{
    rotate_block(dim, src, dst, 32);
}

/*
 * naive_rotate - The naive baseline version of rotate
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel* src, pixel* dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel* src, pixel* dst)
{
    rotate_v3(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate_v1, rotate_v1_descr);
    add_rotate_function(&rotate_v2, rotate_v2_descr);
    add_rotate_function(&rotate_v3, rotate_v3_descr);
    add_rotate_function(&rotate_v4, rotate_v4_descr);
    /* ... Register additional test functions here */
}

/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum* sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum* sum, pixel p)
{
    sum->red += (int)p.red;
    sum->green += (int)p.green;
    sum->blue += (int)p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel* current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short)(sum.red / sum.num);
    current_pixel->green = (unsigned short)(sum.green / sum.num);
    current_pixel->blue = (unsigned short)(sum.blue / sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel* src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for (ii = max(i - 1, 0); ii <= min(i + 1, dim - 1); ii++)
        for (jj = max(j - 1, 0); jj <= min(j + 1, dim - 1); jj++)
            accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/
void smooth_block(int dim, pixel* src, pixel* dst, int block)
{
    int i, j, x, y;

    for (i = 0; i < dim; i += block)
        for (j = 0; j < dim; j += block)
            for (x = i; x < i + block; ++x)
                for (y = j; y < j + block; ++y)
                    dst[RIDX(x, y, dim)] = avg(dim, x, y, src);
}

char smooth_v1_descr[] = "smooth_v1: 分块8X8";
void smooth_v1(int dim, pixel* src, pixel* dst)
{
    smooth_block(dim, src, dst, 8);
}

char smooth_v2_descr[] = "smooth_v2: 分块16X16";
void smooth_v2(int dim, pixel* src, pixel* dst)
{
    smooth_block(dim, src, dst, 16);
}

/**
 * @brief 计算一个点平滑后的rgb值
 *
 * @param src 源矩阵
 * @param dst 目的矩阵
 * @param pixels 用于计算的src中的像素点坐标
 * @param len pixels数组长度
 */
void calculate(const pixel* src, pixel* dst, const int pixels[], int len)
{
    int r = 0, g = 0, b = 0;
    for (int i = 0; i < len; ++i) {
        r += src[pixels[i]].red;
        g += src[pixels[i]].green;
        b += src[pixels[i]].blue;
    }
    dst->red = r / len;
    dst->green = g / len;
    dst->blue = b / len;
}

char smooth_v3_descr[] = "smooth_v3: 减少调用max，min函数计算";
void smooth_v3(int dim, pixel* src, pixel* dst)
{
    // 左上角
    int target = 0; // 目标点坐标
    calculate(src, &dst[target], (int[]) { target, target + 1, target + dim, target + 1 + dim }, 4);
    // 上边缘
    for (int j = 1; j < dim - 1; ++j) {
        target = j;
        calculate(src, &dst[target], (int[]) { target - 1, target, target + 1, target - 1 + dim, target + dim, target + 1 + dim }, 6);
    }
    // 右上角
    target = dim - 1;
    calculate(src, &dst[target], (int[]) { target, target - 1, target + dim, target - 1 + dim }, 4);

    // 左边缘
    for (int i = 1; i < dim - 1; ++i) {
        target = RIDX(i, 0, dim);
        calculate(src, &dst[target], (int[]) { target - dim, target + 1 - dim, target, target + 1, target + dim, target + 1 + dim }, 6);
    }

    // 右边缘
    for (int i = 1; i < dim - 1; ++i) {
        target = RIDX(i, dim - 1, dim);
        calculate(src, &dst[target], (int[]) { target - 1 - dim, target - dim, target - 1, target, target - 1 + dim, target + dim }, 6);
    }

    // 普通位置
    for (int i = 1; i < dim - 1; ++i) {
        for (int j = 1; j < dim - 1; ++j) {
            target = RIDX(i, j, dim);
            calculate(src, &dst[target], (int[]) { target - 1 - dim, target - dim, target + 1 - dim, target - 1, target, target + 1, target - 1 + dim, target + dim, target + 1 + dim }, 9);
        }
    }

    // 左下角
    target = RIDX(dim - 1, 0, dim);
    calculate(src, &dst[target], (int[]) { target, target + 1, target - dim, target + 1 - dim }, 4);
    // 下边缘
    for (int j = 1; j < dim - 1; ++j) {
        target = RIDX(dim - 1, j, dim);
        calculate(src, &dst[target], (int[]) { target - 1, target, target + 1, target - 1 - dim, target - dim, target + 1 - dim }, 6);
    }
    // 右下角
    target = RIDX(dim - 1, dim - 1, dim);
    calculate(src, &dst[target], (int[]) { target, target - 1, target - dim, target - 1 - dim }, 4);
}

static inline void calculate_corner(const pixel* src, pixel* dst, const int pix[4])
{
    dst->red = (src[pix[0]].red + src[pix[1]].red + src[pix[2]].red + src[pix[3]].red) / 4;
    dst->green = (src[pix[0]].green + src[pix[1]].green + src[pix[2]].green + src[pix[3]].green) / 4;
    dst->blue = (src[pix[0]].blue + src[pix[1]].blue + src[pix[2]].blue + src[pix[3]].blue) / 4;
}

static inline void calculate_edge(const pixel* src, pixel* dst, const int pix[6])
{
    dst->red = (src[pix[0]].red + src[pix[1]].red + src[pix[2]].red + src[pix[3]].red + src[pix[4]].red + src[pix[5]].red) / 6;
    dst->green = (src[pix[0]].green + src[pix[1]].green + src[pix[2]].green + src[pix[3]].green + src[pix[4]].green + src[pix[5]].green) / 6;
    dst->blue = (src[pix[0]].blue + src[pix[1]].blue + src[pix[2]].blue + src[pix[3]].blue + src[pix[4]].blue + src[pix[5]].blue) / 6;
}

static inline void calculate_normal(const pixel* src, pixel* dst, const int pix[9])
{
    dst->red = (src[pix[0]].red + src[pix[1]].red + src[pix[2]].red + src[pix[3]].red + src[pix[4]].red + src[pix[5]].red + src[pix[6]].red + src[pix[7]].red + src[pix[8]].red) / 9;
    dst->green = (src[pix[0]].green + src[pix[1]].green + src[pix[2]].green + src[pix[3]].green + src[pix[4]].green + src[pix[5]].green + src[pix[6]].green + src[pix[7]].green + src[pix[8]].green) / 9;
    dst->blue = (src[pix[0]].blue + src[pix[1]].blue + src[pix[2]].blue + src[pix[3]].blue + src[pix[4]].blue + src[pix[5]].blue + src[pix[6]].blue + src[pix[7]].blue + src[pix[8]].blue) / 9;
}

char smooth_v4_descr[] = "smooth_v4: 循环展开，并使用内联函数减少函数调用开销";
void smooth_v4(int dim, pixel* src, pixel* dst)
{
    // 左上角
    int target = 0; // 目标点坐标
    calculate_corner(src, &dst[target], (int[]) { target, target + 1, target + dim, target + 1 + dim });
    // 上边缘
    for (int j = 1; j < dim - 1; ++j) {
        target = j;
        calculate_edge(src, &dst[target], (int[]) { target - 1, target, target + 1, target - 1 + dim, target + dim, target + 1 + dim });
    }
    // 右上角
    target = dim - 1;
    calculate_corner(src, &dst[target], (int[]) { target, target - 1, target + dim, target - 1 + dim });

    // 左边缘
    for (int i = 1; i < dim - 1; ++i) {
        target = RIDX(i, 0, dim);
        calculate_edge(src, &dst[target], (int[]) { target - dim, target + 1 - dim, target, target + 1, target + dim, target + 1 + dim });
    }

    // 右边缘
    for (int i = 1; i < dim - 1; ++i) {
        target = RIDX(i, dim - 1, dim);
        calculate_edge(src, &dst[target], (int[]) { target - 1 - dim, target - dim, target - 1, target, target - 1 + dim, target + dim });
    }

    // 普通位置
    for (int i = 1; i < dim - 1; ++i) {
        for (int j = 1; j < dim - 1; ++j) {
            target = RIDX(i, j, dim);
            calculate_normal(src, &dst[target], (int[]) { target - 1 - dim, target - dim, target + 1 - dim, target - 1, target, target + 1, target - 1 + dim, target + dim, target + 1 + dim });
        }
    }

    // 左下角
    target = RIDX(dim - 1, 0, dim);
    calculate_corner(src, &dst[target], (int[]) { target, target + 1, target - dim, target + 1 - dim });
    // 下边缘
    for (int j = 1; j < dim - 1; ++j) {
        target = RIDX(dim - 1, j, dim);
        calculate_edge(src, &dst[target], (int[]) { target - 1, target, target + 1, target - 1 - dim, target - dim, target + 1 - dim });
    }
    // 右下角
    target = RIDX(dim - 1, dim - 1, dim);
    calculate_corner(src, &dst[target], (int[]) { target, target - 1, target - dim, target - 1 - dim });
}
/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel* src, pixel* dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel* src, pixel* dst)
{
    smooth_v4(dim, src, dst);
}

/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions()
{
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth_v1, smooth_v1_descr);
    add_smooth_function(&smooth_v2, smooth_v2_descr);
    add_smooth_function(&smooth_v3, smooth_v3_descr);
    add_smooth_function(&smooth_v4, smooth_v4_descr);
    /* ... Register additional test functions here */
}

