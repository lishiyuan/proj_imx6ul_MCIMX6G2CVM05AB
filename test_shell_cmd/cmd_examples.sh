#gpio0
echo 0 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio0/direction
echo 0 > /sys/class/gpio/gpio0/value
echo 1 > /sys/class/gpio/gpio0/value

#gpio1
echo 1 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio1/direction
echo 0 > /sys/class/gpio/gpio1/value
echo 1 > /sys/class/gpio/gpio1/value

#gpio2
echo 2 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio2/direction
echo 0 > /sys/class/gpio/gpio2/value
echo 1 > /sys/class/gpio/gpio2/value

#gpio1
echo 3 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio3/direction
echo 0 > /sys/class/gpio/gpio3/value
echo 1 > /sys/class/gpio/gpio3/value









#echo 0 > /sys/class/gpio/unexport
