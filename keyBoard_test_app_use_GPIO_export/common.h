/* 版权声明
 * 功能     ：通用功能函数定义接口、宏等声明
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/16 20:47
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

/*
 * common micro
 */
#define VOS_TRUE  1
#define VOS_FALSE 0

#define VOS_OK    0
#define VOS_NOK   1
#define VOS_ERROR (-1)

/*
 * common function
 */
int IsFileExist(const char* path);

#endif

