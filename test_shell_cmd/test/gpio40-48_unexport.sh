
for num in 40 41 42 43 44 45 46 47
do
    echo $num > /sys/class/gpio/unexport
    echo "gpio $num unexport."
done
echo complete
