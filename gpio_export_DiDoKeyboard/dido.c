/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : /mnt/hgfs/github/proj_imx6ul_MCIMX6G2CVM05AB/do/do.c
 * 创建时间 ：2021/01/06 12:58
 */

#include "common.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DO_CHANNEL_GPIO_NUM_MIN 40
#define DO_CHANNEL_GPIO_NUM_MAX 47
#define DO_CHANNEL_NUM_MIN 0
#define DO_CHANNEL_NUM_MAX 7

/*
 * DO功能初始化
 */
static void DO_Init(void)
{
    int channel;

    /* 创建export节点/sys/class/gpio/gpioNum */
    for (channel = DO_CHANNEL_GPIO_NUM_MIN; channel <= DO_CHANNEL_GPIO_NUM_MAX; channel++) {
        GPIO_CreateNode(channel);
    }

    /* 设置为输出模式，且全部默认输出0 */
    for (channel = DO_CHANNEL_GPIO_NUM_MIN; channel <= DO_CHANNEL_GPIO_NUM_MAX; channel++) {
        GPIO_SetDirection(channel, GPIO_DIRECTION_OUTPUT);
        GPIO_SetLevel(channel, 0);
    }
}

/*
 * 设置输出DO通道和高低电平
 * 命令格式：
 *     ./xxx [channel:0-7] [level:0/1]
 */
int DO_Enable(int channel, int level)
{
    if ((channel > DO_CHANNEL_NUM_MAX) || (channel < DO_CHANNEL_NUM_MIN)) {
        printf("do channel error.\r\n");
        return VOS_ERROR;
    }

    if ((level != 0) && (level != 1)) {
        printf("do level error.\r\n");
        return VOS_ERROR;
    }

    /* channel0 - 7 对应 gpio40 - 47 */
    channel = channel + DO_CHANNEL_GPIO_NUM_MIN;
    DEBUG_INFO("channel = %d, level = %d\r\n", channel, level);

    DO_Init();
    GPIO_SetLevel(channel, level);

    return level;
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
