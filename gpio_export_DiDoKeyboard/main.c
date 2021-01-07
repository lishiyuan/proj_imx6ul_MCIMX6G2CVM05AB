/* 版权声明
 * 功能     ：主函数逻辑
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/19 14:10
 */

#include "test.h"
#include "dido.h"
#include "keyboard.h"
#include <stdio.h>

/*
 * 功能：3个功能轮询：keyboard、di、do
 *       由于三个功能共用8个gpio，所以分时复用，
 *       硬件电路上又由使能芯片控制，故每个功能
 *       激活时均需要调用使能函数
 */

#define ENABLE_KEYBOARD 1
#define ENABLE_DI       1
#define ENABLE_DO       1

int main(int argc, char *argv[])
{
    /*keyboard功能*/
    KeyboardTest();

    /*do功能*/
    //DO_Test(argc, argv);

    /*di功能*/
    //DI_Test(argc, argv);

    return 0;
}
