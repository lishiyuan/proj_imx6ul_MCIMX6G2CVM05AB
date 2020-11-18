#!/bin/bash
# gpio list gpio (bank-1)*32 + nr
# 操作命令：
#     备注：先赋予该脚本可执行权限再执行下述命令。
#     ./key_gpio_4pins_set_mode_output_AND_set_value 0/1

for num in 48 49 50 51
do
    #gpio设备文件的【文件夹】不存在才创建
    if [ ! -d "/sys/class/gpio/gpio$num" ];then
        echo Exporting pin $num.
        echo $num> /sys/class/gpio/export
        echo out > /sys/class/gpio/gpio$num/direction
    else
        echo $1 > /sys/class/gpio/gpio$num/value
    fi
done
echo complete


#echo $num> /sys/class/gpio/unexport
