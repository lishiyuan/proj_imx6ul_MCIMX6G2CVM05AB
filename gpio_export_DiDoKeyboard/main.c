/* 版权声明
 * 功能     ：主函数逻辑
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/19 14:10
 */

#include "common.h"
#include "test.h"
#include "dido.h"
#include "gpio.h"
#include "keyboard.h"
#include <stdio.h>
#include <unistd.h>

/*
 * 控制电路功能的几个gpio序号
 * 备注：软件实现要求控制功能的gpio序号必须连续
 *
 * 3个功能对应的io电平编号
 * 备注：从低位到高位分别表示gpio序号从小到大
 * 即  ：bit3 --> gpio3
 *       bit2 --> gpio2
 *       bit1 --> gpio1
 *       bit0 --> gpio0
 * 待电路修正后，只需要修改此处代码即可
 */
#define GPIO1_IO00 0
#define GPIO1_IO01 1
#define GPIO1_IO02 2
#define GPIO1_IO03 3
#define MODE_GPIO_NUM_MIN 0
#define MODE_GPIO_NUM_MAX 3
#define MODE_KEYBOARD     0b0101
#define MODE_DI           0b1001
#define MODE_DO           0b1110

/*
 * 功能：使能KeyBoard
 */
void ModeEnable(char mode)
{
    int val;

    /* 创建节点，设置为输出模式 */
    for (val = MODE_GPIO_NUM_MIN; val <= MODE_GPIO_NUM_MAX; val++) {
        GPIO_CreateNode(val);
        GPIO_SetDirection(val, GPIO_DIRECTION_OUTPUT);
    }

    /* 设置IO电平，激活对应模式 */
    for (val = MODE_GPIO_NUM_MIN; val <= MODE_GPIO_NUM_MAX; val++) {
        GPIO_SetLevel(val, 1 & (mode >> (val - MODE_GPIO_NUM_MIN)));

        lsydebug("Set gpio%d val to %d\r\n", val, 1 & (mode >> (val - MODE_GPIO_NUM_MIN)));
    }

    /* 调用对应模式功能函数 */
    if (mode == MODE_KEYBOARD) {
        GetPressKey(keyBoard);
    }

    if (mode == MODE_DI) {

    }

    if (mode == MODE_DO) {

    }
}

/*
 * 功能：3个功能：keyboard、di、do
 */
int main(int argc, char *argv[])
{
    printf("Start.\r\n");

    lsydebug("----\r\n");
    ModeEnable(MODE_KEYBOARD);

    /*lsydebug("----\r\n");*/
    /*ModeEnable(MODE_DI);*/

    /*lsydebug("----\r\n");*/
    /*ModeEnable(MODE_DO);*/

    return 0;
}
