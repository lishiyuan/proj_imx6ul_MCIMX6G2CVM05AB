/* 版权声明
 * 功能     ：通用功能函数定义接口
 * 作者     ：lsy
 * 文件路径 :
 * 创建时间 ：2020/11/16 20:47
 */

#include "common.h"
#include <stdio.h>
#include <unistd.h>

/*
 * 功能：判断某个文件是否存在(包括普通文件和目录文件)
 * 返回值：1：存在
 *         0：不存在
 */
int IsFileExist(const char* path)
{
    if (access(path, F_OK) == 0) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}

