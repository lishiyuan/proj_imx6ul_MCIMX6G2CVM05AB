/* 版权声明
 * 功能     ：主函数逻辑
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/19 14:10
 */

#include "keyboard.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    KeyBoard keyBoard;

    OKIMX6UL_KeyBoardEnable(&keyBoard);

    return 0;
}
