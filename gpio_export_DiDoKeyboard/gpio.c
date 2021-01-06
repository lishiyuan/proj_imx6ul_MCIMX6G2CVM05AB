/* 版权声明
 * 功能     ：实现gpio相关操作函数
 * 作者     ：lsy
 * 文件路径 :
 * 创建时间 ：2020/11/16 20:47
 */

#include "gpio.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 功能：根据GPIO序号创建对应的设备驱动文件
 */
void GPIO_CreateNode(int gpioNum)
{
    char gpioNodePath[GPIO_DEV_PATH_LEN_MAX];
    char cmd[GPIO_CREATE_DEV_NODE_CMD_LEN];

    if ((gpioNum < GPIO_PIN_NUM_MIN) || (gpioNum > GPIO_PIN_NUM_MAX)) {
        printf("Illegal pin num.\r\n");
        return ;
    }

    memset(gpioNodePath, 0, sizeof(gpioNodePath));
    snprintf(gpioNodePath, sizeof(gpioNodePath) - 1, "/sys/class/gpio/gpio%d", gpioNum);  /* 避免内存泄露 */

    /* 当节点不存在时，才创建节点 */
    if (IsFileExist(gpioNodePath) != VOS_TRUE) {
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd) -1, "echo %d > /sys/class/gpio/export", gpioNum); /* 避免内存泄露 */
        system(cmd);
    }

    memset(gpioNodePath, 0, sizeof(gpioNodePath));
    memset(cmd, 0, sizeof(cmd));
}

/*
 * 功能  ：设置GPIO引脚为输出模式
 * 参数  ：gpioNum   ：GPIO序号
 *         direction ：in/on输出模式或输出模式
 * 返回值：VOS_OK    : 设置成功
 *         其他      : 设置失败
 */
int GPIO_SetDirection(int gpioNum, char *direction)
{
    char cmd[GPIO_CREATE_DEV_NODE_CMD_LEN];

    if ((gpioNum < GPIO_PIN_NUM_MIN) || (gpioNum > GPIO_PIN_NUM_MAX)) {
        printf("Illegal pin num.\r\n");
        return VOS_ERROR;
    }

    if (direction == NULL) {
        printf("Illegal direction.\r\n");
        return VOS_ERROR;
    } else {
        if ((strncmp(GPIO_DIRECTION_INPUT, direction, strlen(GPIO_DIRECTION_INPUT)) != 0) &&
            (strncmp(GPIO_DIRECTION_OUTPUT, direction, strlen(GPIO_DIRECTION_OUTPUT)) != 0)) {
            printf("gpio direction str error!\r\n");
            return VOS_ERROR;
        }
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) -1, "echo %s > /sys/class/gpio/gpio%d/direction", direction, gpioNum); /* 避免内存泄露 */
    system(cmd);
    memset(cmd, 0, sizeof(cmd));

    return VOS_OK;
}

/*
 * 功能  ：设置GPIO输出电平
 * 参数  ：gpioNum : GPIO序号
 *         level   : 1/0 高/低电平
 */
void GPIO_SetLevel(int gpioNum, int level)
{
    char cmd[GPIO_CREATE_DEV_NODE_CMD_LEN];

    if ((gpioNum < GPIO_PIN_NUM_MIN) || (gpioNum > GPIO_PIN_NUM_MAX)) {
        printf("Illegal pin num.\r\n");
        return ;
    }

    if ((level != GPIO_LEVEL_HIGH) && (level != GPIO_LEVEL_LOW)) {
        printf("level error\r\n");
        return ;
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) -1, "echo %d > /sys/class/gpio/gpio%d/value", level, gpioNum); /* 避免内存泄露 */
    system(cmd);
    memset(cmd, 0, sizeof(cmd));
}

/*
 * 功能  ：获取GPIO电平
 * 返回值：GPIO电平
 */
int GPIO_GetLevel(int gpioNum)
{
    FILE *fp = NULL;
    int level;
    char ch;
    char gpioValuePath[GPIO_LEVEL_PATH_LEN];

    if ((gpioNum < GPIO_PIN_NUM_MIN) || (gpioNum > GPIO_PIN_NUM_MAX)) {
        printf("Illegal pin num.\r\n");
        return VOS_ERROR;
    }

    memset(gpioValuePath, 0, sizeof(gpioValuePath));
    snprintf(gpioValuePath, sizeof(gpioValuePath) -1, "/sys/class/gpio/gpio%d/value", gpioNum); /* 避免内存泄露 */
    fp = fopen(gpioValuePath, "r");
    if (fp < 0) {
        printf("fopen %s error!\r\n", gpioValuePath);
        return VOS_ERROR;
    }

    fread(&ch, 1, 1, fp); /* level只是0或1，只有一个元素，故参数2参数3均为1 */
    level = atoi(&ch);

    memset(gpioValuePath, 0, sizeof(gpioValuePath));
    fclose(fp);

    return level;
}

