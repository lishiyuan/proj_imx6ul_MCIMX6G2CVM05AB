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

#define FALSE 0
#define TRUE  1

#define DO_CHANNEL_GPIO_NUM_MIN 40
#define DO_CHANNEL_GPIO_NUM_MAX 47
#define DO_CHANNEL_NUM_MIN 0
#define DO_CHANNEL_NUM_MAX 7

/*
 * 功能：DO功能初始化
 * 参数：direction：dido模式
 *           out：表示do模式
 *           in ：表示di模式
 */
static void DIDO_Init(char *direction)
{
    int channel;

    /* 创建export节点/sys/class/gpio/gpioNum */
    for (channel = DO_CHANNEL_GPIO_NUM_MIN; channel <= DO_CHANNEL_GPIO_NUM_MAX; channel++) {
        GPIO_CreateNode(channel);
    }

    /* 设置为输出模式，且全部默认输出0 */
    for (channel = DO_CHANNEL_GPIO_NUM_MIN; channel <= DO_CHANNEL_GPIO_NUM_MAX; channel++) {
        GPIO_SetDirection(channel, direction);
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
    static int isDoInitializedFlag = FALSE;

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

    /* 只初始化一次即可 */
    if (isDoInitializedFlag != TRUE) {
        DIDO_Init(GPIO_DIRECTION_OUTPUT);
        isDoInitializedFlag = TRUE;
    }

    GPIO_SetLevel(channel, level);

    return level;
}

/*
 * 功能  ：捕获DI通道电平
 * 参数  ：channel：DI通道，范围：0-8
 * 返回值：捕获到的电平
 */
int DI_Enable(int channel)
{
    int level;
    static int isDiInitializedFlag = FALSE;

    if ((channel > DO_CHANNEL_NUM_MAX) || (channel < DO_CHANNEL_NUM_MIN)) {
        printf("do channel error.\r\n");
        return VOS_ERROR;
    }

    /* channel0 - 7 对应 gpio40 - 47 */
    channel = channel + DO_CHANNEL_GPIO_NUM_MIN;

    /* 只初始化一次即可 */
    if (isDiInitializedFlag != TRUE) {
        DIDO_Init(GPIO_DIRECTION_INPUT);
        isDiInitializedFlag = TRUE;
    }

    //return GPIO_GetLevel(channel);

    level = GPIO_GetLevel(channel);
    DEBUG_INFO("channel = %d, level = %d\r\n", channel, level);
    return level;
}

