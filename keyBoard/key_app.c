/* 版权声明
 * 功能     ：
 * 作者     ：lsy
 * 文件路径 : /home/lsy/work/yueRenFeng/shell_key_gpio/key_app_shell/key_app.c
 * 创建时间 ：2020/11/16 20:47
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * 通用宏
 */
#define VOS_TRUE  1
#define VOS_FALSE 0

#define VOS_OK    0
#define VOS_NOK   1
#define VOS_ERROR (-1)

/*
 * pin num range.
 */
#define GPIO_PIN_NUM_MAX 196
#define GPIO_PIN_NUM_MIN 0

/*
 * gpio dev node
 */
#define GPIO_DEV_PATH_LEN_MAX         512
#define GPIO_CREATE_DEV_NODE_CMD_LEN  ((GPIO_DEV_PATH_LEN_MAX) + 64)
#define GPIO_LEVEL_PATH_LEN           ((GPIO_DEV_PATH_LEN_MAX) + 64)
#define GPIO_LEVEL_STR_LEN            1

/*
 * gpio direction
 */
#define GPIO_DIRECTION_OUTPUT   "out"
#define GPIO_DIRECTION_INPUT    "in"

/*
 * gpio out level
 */
#define GPIO_LEVEL_HIGH 1
#define GPIO_LEVEL_LOW  0

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
 * keyboard en.
 */
#define ENABLE_GPIO1_IO00_U4A_74LVC139D_PIN_1E  0
#define ENABLE_GPIO1_IO01_U4A_74LVC139D_PIN_1A1 1
#define ENABLE_GPIO1_IO02_U18_74HCT541_PIN_OE2  2
#define ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2  3

/*
 * keyboard
 *
 * 矩阵键盘使用到的gpio序号：
 *     从             ： GPIO2_8 - GPIO2_19
 *     对应GPIO num为 ：   40    -    51
 *     共计           ： 12个GPIO
 *
 * 矩阵键盘结构为：
 *                横向GPIO为输出模式，默认输出低电平，轮流且每次只能有一个GPIO输出1
 *                纵向GPIO为输入模式，用于捕获电平。
 *
 *                纵向几个GPIO均有下拉，当按键未按下时，捕获到的始终为低电平。
 *                当按键按下,横向输出GPIO和纵向输出GPIO被按键导通，则会捕获到某个横向GPIO的高电平，则可以判断具体的某个按键按下。
 *
 *                          GPIO2_11     GPIO2_12     GPIO2_10     GPIO2_8     GPIO2_9     GPIO2_13     GPIO2_14     GPIO2_15
 *                          (43)         (44)         (42)         (40)        (41)        (45)         (46)         (47)
 *                          pin8         pin7         pin6         pin5        pin4        pin3         pin2         pin1
 *                                                                                                                            
 * GPIO2_16(48)   pin9      left         up           F1           F-X          7            4           1            M
 *                                                                                                                            
 * GPIO2_17(49)   pin10     down         right        F2           F-O          8            5           2            0
 *                                                                                                                            
 * GPIO2_19(51)   pin11      x            x           enter        F-I          9            6           3            dot
 *                                                                                                                            
 * GPIO2_18(50)   pin12      x            x           x            x            x            x            x           x
 *                                                                                                                            
 */
typedef struct {
    char left;  /* GPIO2_16(48)  +  GPIO2_11(43) */
    char down;  /* GPIO2_17(49)  +  GPIO2_11(43) */
    char up;    /* GPIO2_16(48)  +  GPIO2_12(44) */
    char right; /* GPIO2_17(49)  +  GPIO2_12(44) */
    char f1;    /* GPIO2_16(48)  +  GPIO2_10(42) */
    char f2;    /* GPIO2_17(49)  +  GPIO2_10(42) */
    char enter; /* GPIO2_19(51)  +  GPIO2_10(42) */
    char fx;    /* GPIO2_16(48)  +  GPIO2_08(40) */
    char fo;    /* GPIO2_17(49)  +  GPIO2_08(40) */
    char fi;    /* GPIO2_19(51)  +  GPIO2_08(40) */
    char num7;  /* GPIO2_16(48)  +  GPIO2_09(41) */
    char num8;  /* GPIO2_17(49)  +  GPIO2_09(41) */
    char num9;  /* GPIO2_19(51)  +  GPIO2_09(41) */
    char num4;  /* GPIO2_16(48)  +  GPIO2_13(45) */
    char num5;  /* GPIO2_17(49)  +  GPIO2_13(45) */
    char num6;  /* GPIO2_19(51)  +  GPIO2_13(45) */
    char num1;  /* GPIO2_16(48)  +  GPIO2_14(46) */
    char num2;  /* GPIO2_17(49)  +  GPIO2_14(46) */
    char num3;  /* GPIO2_19(51)  +  GPIO2_14(46) */
    char m;     /* GPIO2_16(48)  +  GPIO2_15(47) */
    char num0;  /* GPIO2_17(49)  +  GPIO2_15(47) */
    char dot;   /* GPIO2_19(51)  +  GPIO2_15(47) */
} KeyBoard;

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

/*
 * 功能：创建KEY对应GPIO设备驱动文件
 */
void GPIO_CreatNode(int gpioNum)
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
 * 参数  ：gpioNum：
 *         mode：in/on输出模式或输出模式
 * 返回值：VOS_OK :设置成功
 *         其他   :设置失败
 */
int GPIO_SetDirection(int gpioNum, char *mode)
{
    char cmd[GPIO_CREATE_DEV_NODE_CMD_LEN];

    if ((gpioNum < GPIO_PIN_NUM_MIN) || (gpioNum > GPIO_PIN_NUM_MAX)) {
        printf("Illegal pin num.\r\n");
        return VOS_ERROR;
    }

    if (mode == NULL) {
        printf("Illegal mode.\r\n");
        return VOS_ERROR;
    } else {
        if ((strncmp(GPIO_DIRECTION_INPUT, mode, strlen(GPIO_DIRECTION_INPUT)) != 0) &&
            (strncmp(GPIO_DIRECTION_OUTPUT, mode, strlen(GPIO_DIRECTION_OUTPUT)) != 0)) {
            printf("mode error!\r\n");
            return VOS_ERROR;
        }
    }

    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd) -1, "echo %s > /sys/class/gpio/gpio%d/direction", mode, gpioNum); /* 避免内存泄露 */
    system(cmd);
    memset(cmd, 0, sizeof(cmd));

    return VOS_OK;
}

/*
 * 功能  ：设置gpio输出电平
 * 参数  ：gpioNum: GPIO序号
 *         level  : 0、1高低电平
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

    //printf("The gpio node create success!\r\n");
}

/*
 * 功能  ：获取gpio电平
 * 返回值：gpio电平
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
static void GetPressKey(KeyBoard *keyBoard)
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
        GPIO_CreatNode(i);
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

/*
 * 功能：使能KeyBoard
 */
void OKIMX6UL_KeyBoardEnable(KeyBoard *keyBoard)
{
    /* 74LVC139D */
    /* GPIO1_IO00 */
    GPIO_CreatNode(ENABLE_GPIO1_IO00_U4A_74LVC139D_PIN_1E);
    GPIO_SetDirection(ENABLE_GPIO1_IO00_U4A_74LVC139D_PIN_1E, GPIO_DIRECTION_OUTPUT);
    GPIO_SetLevel(ENABLE_GPIO1_IO00_U4A_74LVC139D_PIN_1E, GPIO_LEVEL_LOW);
    /* GPIO1_IO01 */
    GPIO_CreatNode(ENABLE_GPIO1_IO01_U4A_74LVC139D_PIN_1A1);
    GPIO_SetDirection(ENABLE_GPIO1_IO01_U4A_74LVC139D_PIN_1A1, GPIO_DIRECTION_OUTPUT);
    GPIO_SetLevel(ENABLE_GPIO1_IO01_U4A_74LVC139D_PIN_1A1, GPIO_LEVEL_HIGH);

    /* 74HCT541 U18 */
    /* GPIO1_IO02 */
    GPIO_CreatNode(ENABLE_GPIO1_IO02_U18_74HCT541_PIN_OE2);
    GPIO_SetDirection(ENABLE_GPIO1_IO02_U18_74HCT541_PIN_OE2, GPIO_DIRECTION_OUTPUT);
    GPIO_SetLevel(ENABLE_GPIO1_IO02_U18_74HCT541_PIN_OE2, GPIO_LEVEL_HIGH);

    /* 74HCT541 U16 */
    /* GPIO1_IO03 */
    GPIO_CreatNode(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2);
    GPIO_SetDirection(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2, GPIO_DIRECTION_OUTPUT);
    GPIO_SetLevel(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2, GPIO_LEVEL_LOW);

    /* KeyBoard start */
    GetPressKey(keyBoard);
}

/*
 * 功能：禁用KeyBoard
 */
void OKIMX6UL_KeyBoardDisable(KeyBoard *keyBoard)
{
    /* GPIO1_IO03 */
    GPIO_CreatNode(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2);
    GPIO_SetDirection(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2, GPIO_DIRECTION_OUTPUT);
    GPIO_SetLevel(ENABLE_GPIO1_IO03_U16_74HCT541_PIN_OE2, GPIO_LEVEL_HIGH);

    memset(keyBoard, 0, sizeof(KeyBoard));  /* 注意：sizeof参数是数据类型，一定不要是指针变量 */
}

int main(int argc, char *argv[])
{
    KeyBoard keyBoard;

    OKIMX6UL_KeyBoardEnable(&keyBoard);

    return 0;
}

