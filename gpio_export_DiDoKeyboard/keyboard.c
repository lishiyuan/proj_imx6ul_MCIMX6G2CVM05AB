/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : 
 * 创建时间 ：2020/11/16 20:47
 */

#include "keyboard.h"
#include "gpio.h"
#include "common.h"
#include <string.h>
#include <unistd.h>

/*
 * key gpio output pin.
 */
#define KEY_OUTPUT_PIN_NUMS      4
#define KEY_OUTPUT_MODE_PIN1_NUM 48
#define KEY_OUTPUT_MODE_PIN2_NUM 49
#define KEY_OUTPUT_MODE_PIN3_NUM 50  /* 该行GPIO未使用 */
#define KEY_OUTPUT_MODE_PIN4_NUM 51

/*
 * key gpio input pin.
 */
#define KEY_INPUT_PIN_NUMS      4
#define KEY_INPUT_MODE_PIN1_NUM 40
#define KEY_INPUT_MODE_PIN2_NUM 41
#define KEY_INPUT_MODE_PIN3_NUM 42
#define KEY_INPUT_MODE_PIN4_NUM 43
#define KEY_INPUT_MODE_PIN5_NUM 45
#define KEY_INPUT_MODE_PIN7_NUM 46
#define KEY_INPUT_MODE_PIN8_NUM 47


/*
 * 功能：匹配按下的按键，并打印用户交互信息
 */
static void SetKeyBoardFlag(KeyBoard *keyBoard, int gpioScanRow, int gpioScanCol)
{
    memset(keyBoard, 0, sizeof(KeyBoard));

    if ((gpioScanRow == 48) && (gpioScanCol == 43)) {
        keyBoard->left = 1;
        printf("left pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 44)) {
        keyBoard->up = 1;
        printf("up pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 42)) {
        keyBoard->f1 = 1;
        printf("f1 pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 40)) {
        keyBoard->fx = 1;
        printf("fx pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 41)) {
        keyBoard->num7 = 1;
        printf("num7 pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 45)) {
        keyBoard->num4 = 1;
        printf("num4 pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 46)) {
        keyBoard->num1 = 1;
        printf("num1 pressed!\r\n");
    } else if ((gpioScanRow == 48) && (gpioScanCol == 47)) {
        keyBoard->m = 1;
        printf("m pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 43)) {
        keyBoard->down = 1;
        printf("down pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 44)) {
        keyBoard->right = 1;
        printf("right pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 42)) {
        keyBoard->f2 = 1;
        printf("f2 pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 40)) {
        keyBoard->fo = 1;
        printf("fo pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 41)) {
        keyBoard->num8 = 1;
        printf("num8 pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 45)) {
        keyBoard->num5 = 1;
        printf("num5 pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 46)) {
        keyBoard->num2 = 1;
        printf("num2 pressed!\r\n");
    } else if ((gpioScanRow == 49) && (gpioScanCol == 47)) {
        keyBoard->num0 = 1;
        printf("num0 pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 42)) {
        keyBoard->enter = 1;
        printf("enter pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 40)) {
        keyBoard->fi = 1;
        printf("fi pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 41)) {
        keyBoard->num9 = 1;
        printf("num9 pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 45)) {
        keyBoard->num6 = 1;
        printf("num6 pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 46)) {
        keyBoard->num3 = 1;
        printf("num3 pressed!\r\n");
    } else if ((gpioScanRow == 51) && (gpioScanCol == 47)) {
        keyBoard->dot = 1;
        printf("dot pressed!\r\n");
    } else {
        printf("UNKNOW key!\r\n");
    }
}

/*
 * 功能：获取按下的按键，
 * 参数：keyBoard为输出参数，结构体，对应成员置1时表示按键按下。
 */
void GetPressKey(KeyBoard *keyBoard)
{
    int i;
    int row;
    int col;

    if (keyBoard == NULL) {
        printf("Struct keyBoard can't be NULL!\r\n");
        return ;
    }

    /* 1. 创建所有GPIO节点 */
    for (i = 40; i <= 51; i++) {
        GPIO_CreateNode(i);
    }

    /* 2. GPIO48 - 51设置为输出模式，且默认均输出低电平 */
    for (row = 48; row <= 51; row++) {
        if (GPIO_SetDirection(row, GPIO_DIRECTION_OUTPUT) != VOS_OK) {
            printf("set gpio mode failed!\r\n");
            return ;
        }

        GPIO_SetLevel(row, GPIO_LEVEL_LOW);  /* 默认低电平 */
    }

    /* 3. GPIO40 - 47设置为输入模式 */
    for (col = 40; col <= 47; col++) {
        if (GPIO_SetDirection(col, GPIO_DIRECTION_INPUT) != VOS_OK) {
            printf("set gpio mode failed!\r\n");
            return ;
        }
    }

    /* 4. GPIO48 - 51轮流且同一时刻仅有一个引脚输出高电平，GPIO40 - 47进行高电平捕获 */
    while (1) {
        /*
         * row:
         * 1 ---> 0 ---> 0 ---> 0
         * 0      1      0      0
         * 0      0      1      0
         * 0      0      0      1
         */
        i = 48;
        row = 48;
        col = 40;

        for (row = 48; row <= 51; row++) {
            /* 先将所有行GPIO全置零，然后将当前扫描的行GPIO置1 */
            for (i = 48; i <= 51; i++) {
                GPIO_SetLevel(i, GPIO_LEVEL_LOW);
            }
            GPIO_SetLevel(row, GPIO_LEVEL_HIGH);

            /* 扫描列GPIO输入 */
            for (col = 40; col <= 47; col++) {
                if (GPIO_GetLevel(col) == GPIO_LEVEL_HIGH) {
                    while (GPIO_GetLevel(col) == GPIO_LEVEL_HIGH); /* 等待按键弹起才算一次按键有效，否则阻塞等待 */

                    printf("\r\n");
                    printf("row : GPIO%d: GPIO2_%d\r\n", row, row - 32);
                    printf("col : GPIO%d: GPIO2_%d\r\n", col, col - 32);
                    SetKeyBoardFlag(keyBoard, row, col);
                    printf("-----------------\r\n");
                }
            }

            usleep(20000); /* 每20毫秒扫描一行，时间再长键盘灵敏度会变差 */
        }
    }
}


