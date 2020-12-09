/* 版权声明
 * 功能     ：声明gpio相关操作函数以及相关pin定义宏等
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/16 20:47
 */

#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>

/*
 * gpio pin num range.
 */
#define GPIO_PIN_NUM_MAX 196
#define GPIO_PIN_NUM_MIN 0

/*
 * gpio dev node
 */
#define GPIO_DEV_PATH_LEN_MAX         512
#define GPIO_CREATE_DEV_NODE_CMD_LEN  ((GPIO_DEV_PATH_LEN_MAX) + 64)
#define GPIO_LEVEL_PATH_LEN           ((GPIO_DEV_PATH_LEN_MAX) + 64)
#define GPIO_LEVEL_STR_LEN            1

/*
 * gpio direction
 */
#define GPIO_DIRECTION_OUTPUT   "out"
#define GPIO_DIRECTION_INPUT    "in"

/*
 * gpio out level
 */
#define GPIO_LEVEL_HIGH 1
#define GPIO_LEVEL_LOW  0

/*
 * gpio function
 */
void GPIO_CreatNode(int gpioNum);
int GPIO_SetDirection(int gpioNum, char *direction);
void GPIO_SetLevel(int gpioNum, int level);
int GPIO_GetLevel(int gpioNum);

#endif

