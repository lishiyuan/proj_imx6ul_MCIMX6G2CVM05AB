/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/16 20:47
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdio.h>

/*
 * keyboard
 *
 * 矩阵键盘使用到的gpio序号：
 *     从             ： GPIO2_8 - GPIO2_19
 *     对应GPIO num为 ：   40    -    51
 *     共计           ： 12个GPIO
 *
 * 矩阵键盘结构为：
 *                横向GPIO为输出模式，默认输出低电平，轮流且每次只能有一个GPIO输出1
 *                纵向GPIO为输入模式，用于捕获电平。
 *
 *                纵向几个GPIO均有下拉，当按键未按下时，捕获到的始终为低电平。
 *                当按键按下,横向输出GPIO和纵向输出GPIO被按键导通，则会捕获到某个横向GPIO的高电平，则可以判断具体的某个按键按下。
 *
 *                          GPIO2_11     GPIO2_12     GPIO2_10     GPIO2_8     GPIO2_9     GPIO2_13     GPIO2_14     GPIO2_15
 *                          (43)         (44)         (42)         (40)        (41)        (45)         (46)         (47)
 *                          pin8         pin7         pin6         pin5        pin4        pin3         pin2         pin1
 *                                                                                                                            
 * GPIO2_16(48)   pin9      left         up           F1           F-X          7            4           1            M
 *                                                                                                                            
 * GPIO2_17(49)   pin10     down         right        F2           F-O          8            5           2            0
 *                                                                                                                            
 * GPIO2_19(51)   pin11      x            x           enter        F-I          9            6           3            dot
 *                                                                                                                            
 * GPIO2_18(50)   pin12      x            x           x            x            x            x            x           x
 *                                                                                                                            
 */
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
void OKIMX6UL_KeyBoardEnable(KeyBoard *keyBoard);
void OKIMX6UL_KeyBoardDisable(KeyBoard *keyBoard);

#endif

