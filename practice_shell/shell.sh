#!/bin/sh

echo ### string test ############################
echo "who are you?:" $USER
echo "where is your home?:" $HOME

var1="Korean"
echo "what's your nationality?:"$var1

echo ### calculation test #######################
num1=100
num2=100
sum=`expr $num1 + 200` # which is at the left of '!'
echo $sum

echo ### variable read test ######################
echo "what is your age?:"
read age
echo "age: " $age

echo ### parameter test #########################
echo "parameter1:" $0
echo "parameter2:" $1
echo "parameter3:" $2

echo ### if-else test ###########################
echo "num1=" $1
echo "num2=" $2

echo "num1 = num2?"

judgement="-eq"
# a -eq b : a and b are the same
# a -ne b : a and be are not the same
# a -gt b : a is greater than b
# a -ge b : a not smaller than b
# a -lt b : a is smaller than b
# a -le b : a is not greater than b
# ! : not

if [ $1 $judgement $2 ]
then 
	echo "true"
else
	echo "false"
fi

# testing?

exit 0
