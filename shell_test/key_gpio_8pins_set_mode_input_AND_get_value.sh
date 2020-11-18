#!/bin/bash
# gpio list gpio (bank-1)*32 + nr
# pin_num: pin40 - pin47 (40 41 42 43 44 45 46 47)

for num in 40 41 42 43 44 45 46 47
do
    #gpio设备文件的【文件夹】不存在才创建
    if [ ! -d "/sys/class/gpio/gpio$num" ];then
        echo Exporting pin $num.
        echo $num > /sys/class/gpio/export
        echo in > /sys/class/gpio/gpio$num/direction
    else
        gpioval=`cat /sys/class/gpio/gpio$num/value`
        echo GPIO $num = $gpioval
        echo
    fi
done
echo complete


#echo $num> /sys/class/gpio/unexport
