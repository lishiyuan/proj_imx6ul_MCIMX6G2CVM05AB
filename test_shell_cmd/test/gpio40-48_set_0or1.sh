#usage:
#      ./xxxxx.sh  0/1

for num in 40 41 42 43 44 45 46 47
do
    echo $1 > /sys/class/gpio/gpio$num/value
    echo "set gpio$num level $1."
done
echo complete
