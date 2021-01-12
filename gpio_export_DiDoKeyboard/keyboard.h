/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/16 20:47
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>

typedef struct {
    char left;  /* GPIO2_16(48)  +  GPIO2_11(43) */
    char down;  /* GPIO2_17(49)  +  GPIO2_11(43) */
    char up;    /* GPIO2_16(48)  +  GPIO2_12(44) */
    char right; /* GPIO2_17(49)  +  GPIO2_12(44) */
    char f1;    /* GPIO2_16(48)  +  GPIO2_10(42) */
    char f2;    /* GPIO2_17(49)  +  GPIO2_10(42) */
    char enter; /* GPIO2_19(51)  +  GPIO2_10(42) */
    char fx;    /* GPIO2_16(48)  +  GPIO2_08(40) */
    char fo;    /* GPIO2_17(49)  +  GPIO2_08(40) */
    char fi;    /* GPIO2_19(51)  +  GPIO2_08(40) */
    char num7;  /* GPIO2_16(48)  +  GPIO2_09(41) */
    char num8;  /* GPIO2_17(49)  +  GPIO2_09(41) */
    char num9;  /* GPIO2_19(51)  +  GPIO2_09(41) */
    char num4;  /* GPIO2_16(48)  +  GPIO2_13(45) */
    char num5;  /* GPIO2_17(49)  +  GPIO2_13(45) */
    char num6;  /* GPIO2_19(51)  +  GPIO2_13(45) */
    char num1;  /* GPIO2_16(48)  +  GPIO2_14(46) */
    char num2;  /* GPIO2_17(49)  +  GPIO2_14(46) */
    char num3;  /* GPIO2_19(51)  +  GPIO2_14(46) */
    char m;     /* GPIO2_16(48)  +  GPIO2_15(47) */
    char num0;  /* GPIO2_17(49)  +  GPIO2_15(47) */
    char dot;   /* GPIO2_19(51)  +  GPIO2_15(47) */
} KeyBoard;

/*
 * keyboard function
 */
void GetPressKey(KeyBoard *keyBoard);

#endif

