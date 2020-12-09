# 一、矩阵键盘功能已完成，包含以下内容：

1. 矩阵键盘本身kpp节点的配置 & 矩阵键盘引脚的复用pinctrl节点

2. 矩阵键盘占用引脚的的屏蔽

3. 以太网2，更换为以太网1

4. 备注：需要进入make menuconfig菜单增加编译选项，选中如下几项：

   ```c
   Device Drivers 
   Input device support
       <*>   Event debugging
       [*]   Keyboards
              <*>  GPIO driven matrix keypad support
              <*>  IMX keypad support
              <*>  GPIO Buttons
   ```

   

# 二、关于matrix_keypad.c

该文件为矩阵键盘源码文件，内核中路径为：

```c
drivers/input/keyboard/matrix_keypad.c
```

本matrix_keypad.c文件仅包含调试过程增加的debug信息，源码未做修改。



# 三、调试过程中遇到的坑

备注：此处仅记录大概，详述见博客

1. 设备树中：行、列一定不能反！Linux内核规定matrix功能：<font color=#FF0000B>**行必须为输入，列必须为输出**</font>。

   row-gpios成员为行，

   col-gpios成员为列。

2. 设备树中：linux,keymap属性键值映射，行列也不能反，反了就乱。

   正确格式为：【<font color=#FF0000B>**0x行+列+十六进制键值 ---> 即：左行右列**</font>】，

   **以0x00000069为例：**如下：

   表示第0行第0列按键，映射为0x0069对应的按键。该值可以在<font color=#FF0000B>**include/uapi/linux/input.h**</font>中找到，需要注意，<font color=#FF0000B>该文件中#define键值为十进制</font>。

   |    0x    |   00   |   00   |       0069       |
   | :------: | :----: | :----: | :--------------: |
   | 十六进制 | 行序号 | 列序号 | 按键十六进制键值 |

3. 上述编译选项menuconfig中该打开的功能必须打开。

4. 设备树中：延时一定要调整合适，否则可能会出现按下一个按键，整行按键都被上报到event设备中。

   ```c
   debounce-delay-ms = <20>;   /*防反跳延时，即：去抖延时*/
   col-scan-delay-us = <400>;  /*列扫描延时*/
   ```

5. 设备树中：kpp节点中如果定义了属性：gpio-activelow，就是告诉软件层面【低电平有效】，激活端口时，必须设置为低电平。应用在函数【__activate_col()】中。