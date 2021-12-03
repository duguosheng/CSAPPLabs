/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
 /*
  * Instructions to Students:
  *
  * STEP 1: Read the following instructions carefully.
  */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES :

Replace the "return" statement in each function with one
or more lines of C code that implements the function.Your code
must conform to the following style :

int Funct(arg1, arg2, ...) {
  /* brief description of how your implementation works */
  int var1 = Expr1;
  ...
    int varM = ExprM;

  varJ = ExprJ;
  ...
    varN = ExprN;
  return ExprR;
}

Each "Expr" is an expression using ONLY the following :
1. Integer constants 0 through 255 (0xFF), inclusive.You are
not allowed to use big constants such as 0xffffffff.
2. Function arguments and local variables(no global variables).
3. Unary integer operations !~
4. Binary integer operations & ^| +<< >>

Some of the problems restrict the set of allowed operators even further.
Each "Expr" may consist of multiple operators.You are not restricted to
one operator per line.

You are expressly forbidden to :
1. Use any control constructs such as if, do, while, for, switch, etc.
2. Define or use any macros.
3. Define any additional functions in this file.
4. Call any functions.
5. Use any other operations, such as &&, || , -, or ? :
  6. Use any form of casting.
  7. Use any data type other than int.This implies that you
  cannot use arrays, structs, or unions.


  You may assume that your machine :
1. Uses 2s complement, 32 - bit representations of integers.
2. Performs right shifts arithmetically.
3. Has unpredictable behavior when shifting if the shift amount
is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE :
/*
 * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
 */
int pow2plus1(int x) {
  /* exploit ability of shifts to compute powers of 2 */
  return (1 << x) + 1;
}

/*
 * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
 */
int pow2plus4(int x) {
  /* exploit ability of shifts to compute powers of 2 */
  int result = (1 << x);
  result += 4;
  return result;
}

FLOATING POINT CODING RULES

For the problems that require you to implement floating - point operations,
the coding rules are less strict.You are allowed to use looping and
conditional control.You are allowed to use both ints andunsigneds.
You can use arbitrary integer andunsigned constants.You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to :
1. Define or use any macros.
2. Define any additional functions in this file.
3. Call any functions.
4. Use any form of casting.
5. Use any data type other than int or unsigned.This means that you
cannot use arrays, structs, or unions.
6. Use any floating point data types, operations, or constants.


NOTES:
1. Use the dlc(data lab checker) compiler(described in the handout) to
check the legality of your solutions.
2. Each function has a maximum number of operations(integer, logical,
  or comparison) that you are allowed to use for your implementation
  of the function.The max operator count is checked by dlc.
  Note that assignment('=') is not counted; you may use as many of
  these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
  header comment for each function.If there are any inconsistencies
  between the maximum ops in the writeup andin this file, consider
  this file the authoritative source.

  /*
   * STEP 2: Modify the following functions according the coding rules.
   *
   *   IMPORTANT. TO AVOID GRADING SURPRISES:
   *   1. Use the dlc compiler to check that your solutions conform
   *      to the coding rules.
   *   2. Use the BDD checker to formally verify that your solutions produce
   *      the correct answers.
   */


#endif
   //1
   /*
    * bitXor - x^y using only ~ and &
    *   Example: bitXor(4, 5) = 1
    *   Legal ops: ~ &
    *   Max ops: 14
    *   Rating: 1
    */
  int bitXor(int x, int y) {
  // x&y得到全为1的位，~x&~y得到全为0的位
  // 二者取反后相与得到不相同的位
  return (~(x & y) & ~(~x & ~y));
}
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  // 最小的二进制补码
  return (1 << 31);
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  // 假设1位符号位，三位数据位，最大值为0111
  // x+1=1000 ~x=1000，加1和取反二者相等
  // 但是1111也符合条件，因此要特殊判断
  // 这里有个技巧：判断两个数相等可以使用异或，两数相等异或为0
  // 0111+1=1000 !1000=0 !0=1
  // 1111+1=0000 !0000=1 !1=0
  return !((x + 1) ^ (~x)) & !!(x + 1);
}
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // 不能直接定义0XAAAAAAAA，因此用或运算产生
  int cmp = 0xAA;
  cmp |= cmp << 8;
  cmp |= cmp << 16;
  // 同样用到了异或判断相等的小技巧
  // 先令原数与0xAAAAAAAA，在判断是否与之相等
  return !((x & cmp) ^ cmp);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  // 根据x的补码求-x的补码：按位取反再加1
  // 推导：x+(~x)=-1 x+(-x)=0 ==> -x=~x+1
  return (~x) + 1;
}
//3
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // 0x30 = 0b110000
  // 0x39 = 0b111001
  // 判断第4，5位（从0开始）是否为1
  int con1 = !((x >> 4) ^ 0x3);
  // 利用-10，即-0xa加上后四位判断后四位的范围
  // 利用0xa，按位取反加1得到
  int neg = ~0xa + 1;

  // 若后四位小于等于9则相加后符号位为1
  int con2 = ((x & 0xf) + neg) >> 31;
  return con1 & con2;
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // 判断x是否为0
  int isZero = !x;
  // 求-isZero的补码
  // 若isZero=1，即x等于0，则mask=0xffffffff
  // 若isZero=0，即x不为0，则mask=0x0
  int mask = ~isZero + 1;
  // ~y+1求得-y的补码，再和~mask相与
  // 也就是说当x为0时minusY=-y
  // 否则minusY=0
  int minusY = (~y + 1) & mask;
  // int minusY = ~(y & mask) + 1;
  // 原理同上，但只有当x为1时才需要-z
  int minusZ = (~z + 1) & ~mask;
  // int minusZ = ~(z & ~mask) + 1;
  // 在返回值中，根据x是否为0，将会抵消y或z
  return y + z + minusY + minusZ;
}
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */

int isLessOrEqual(int x, int y) {
  // 注意这里不是真正的符号位，如果符号位为1
  // 由于算术右移结果是0xffffffff
  int sx = x >> 31;
  int sy = y >> 31;
  // 条件1：x为负数但y为正数
  int con1 = !!sx & !sy;
  // 符号相同
  int sameSign = !(sx ^ sy);
  // 求-x，如果x=INT_MIN，那么-x实际上还是INT_MIN
  // 但不影响结果，请看下面
  int minusX = ~x + 1;
  // 条件2：
  // 2.1: 都为正数，若x<=y，那么-x+y>=0，即符号位为0
  //      例如x=2, y=3, -x+y=-2+3=1
  // 2.2: 都为负数，若x<=y，那么依然-x+y>=0，符号位为0
  //      例如x=-3, y=-2, -x+y=3-2=1
  //      当x=INT_MIN时，-x的补码依然是INT_MIN
  //      此时相加会产生负溢出，结果依然为正，股不影响结果
  int con2 = sameSign & !((minusX + y) >> 31);
  return con1 | con2;
}
//4
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  // 获取符号位
  int sx = (x >> 31) & 1;
  // 获取-x的补码
  int minusX = ~x + 1;
  // 获取-x的符号位
  int sMinusX = minusX >> 31;
  // 只有0和INT_MIN取负值后符号位相同
  // 这里用到了一个常用技巧：取反某一位使用异或
  // sx^sMinusX 若符号位相同则为0，然后异或得到1
  // 为了区分0和INT_MIN还需要判断sx，0的sx为0，异或后为1
  // 两个条件相与得到结果
  return ((sx ^ sMinusX ^ 1) & (sx ^ 1));
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int shift16, shift8, shift4, shift2, shift1, shift0;
  // 符号位
  int sx = x >> 31;
  // 正数: sx=0x00...0, x=0b0000...
  //      sx&~x=0  x&~sx=x, 或运算结果为x本身
  // 负数: sx=0xff...f, x=0b1111...
  //      sx&~x=~x  x&~sx=0, 或运算结果为~x
  // 接下来只需要判断最高位的1在哪里
  x = (sx & ~x) | (x & ~sx);
  // 若高16位有1存在，那么!!(x>>16)=1, shiftSize=16
  // 若高16位不存在1，shiftSize=0
  shift16 = !!(x >> 16) << 4;
  // 如果高16位存在1，那么右移16位，来检查高16位的情况
  // 若高16位不存在1，右移无效，检查低16位
  x >>= shift16;

  shift8 = !!(x >> 8) << 3;
  x >>= shift8;

  shift4 = !!(x >> 4) << 2;
  x >>= shift4;

  shift2 = !!(x >> 2) << 1;
  x >>= shift2;

  shift1 = !!(x >> 1);
  x >>= shift1;

  shift0 = x;

  // 最后的1是符号位
  // 正数：例如8，需要4+1位，即0b01000
  // 负数：例如-5，0b1011，求得取反后0b0100中最高位1的位置后还需加1
  return shift16 + shift8 + shift4 + shift2 + shift1 + shift0 + 1;
}
//float
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  int sign = (uf >> 31) & 0x1;  // 符号位
  int exp = (uf >> 23) & 0xff;  // 阶码
  int frac = uf & 0x7fffff;     // 小数位

  // 阶码全部为1的特殊值
  // 若frac=0，表示无穷大；若frac不全为0，表示NaN
  // 这两种情况都可以直接返回
  if (!(exp ^ 0xff))
    return uf;
  else if (!exp)  // 阶码全部为0，小数部分为0.M，直接左移
    return (sign << 31) | (frac << 1);
  else  // 规格化数，小数部分为1.M，将阶码加1
    return (sign << 31) | ((exp + 1) << 23) | frac;
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int sign = (uf >> 31) & 0x1;  // 符号位
  int exp = (uf >> 23) & 0xff;  // 阶码
  int frac = uf & 0x7fffff;     // 小数位
  int realExp = exp - 127;      // 真正的阶数

  // 1.M左移31位会溢出
  if (realExp >= 31)
    return 0x80000000u;
  if (realExp < 0)
    return 0;

  // 规格化数隐藏一位1
  frac |= 1 << 23;
  if (realExp < 23) {
    // realExp小于23时，需要右移舍入部分位
    frac >>= (23 - realExp);
  } else {
    // 大于23时，左移
    frac <<= (realExp - 23);
  }

  return (sign ? -frac : frac);
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  // 阶码为0xfe时，表示幂次为127
  if (x > 127)
    return 0xff << 23; // +INF

  // -126+(-23)最低次幂为-149
  if (x < -149)
    return 0;

  // 规格化数
  if (x >= -126) {
    int exp = x + 127;
    return exp << 23;
  } else {
    // -126+(shift-23)=x  ==>  shift=149+x
    // 阶码自带-126次幂
    // 在23位的小数位中，1左移0位表示2e-23，左移1位表示2e-22
    // 可得: 移位次幂=-23+shift
    int shiftSize = 149 + x;
    return 1 << shiftSize;
  }
}