/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : /mnt/hgfs/github/proj_imx6ul_MCIMX6G2CVM05AB/gpio_export_DiDoKeyboard/test.c
 * 创建时间 ：2021/01/06 14:24
 */


#include "keyBoard.h"
#include "common.h"
#include "gpio.h"
#include "dido.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * di测试代码
 */
int DI_Test(int argc, char *argv[])
{
    int channel;
    int level;

    if (argc != 2) {
        printf("usage:\r\n");
        printf("    ./xxx [channel:0-7] \r\n");
        return VOS_ERROR;
    }

    channel = atoi(argv[1]);
    DEBUG_INFO("channel = %d\r\n", channel);

    while (1) {
        level = DI_Enable(channel);
        printf("channel %d capture level = %d\r\n", channel, level);

        usleep(300);
    }

    return VOS_OK;
}

/*
 * do测试代码
 */
int DO_Test(int argc, char *argv[])
{
    int channel;
    int level;

    if (argc != 3) {
        printf("usage:\r\n");
        printf("    ./xxx [channel:0-7] [level:0/1]\r\n");
        return VOS_ERROR;
    }

    channel = atoi(argv[1]);
    level = atoi(argv[2]);
    DO_Enable(channel, level);

    return VOS_OK;
}

/*
 * keyboard test
 */
void KeyboardTest(void)
{
    KeyBoard keyBoard;

    OKIMX6UL_KeyBoardEnable(&keyBoard);
}
