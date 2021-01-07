for num in 40 41 42 43 44 45 46 47
do
    if [ ! -d "/sys/class/gpio/gpio$num" ];then
        echo Exporting pin $num.
        echo $num > /sys/class/gpio/export
        echo out > /sys/class/gpio/gpio$num/direction
    else
        gpioval=`cat /sys/class/gpio/gpio$num/value`
        echo GPIO $num = $gpioval
        echo
    fi
done
echo complete
