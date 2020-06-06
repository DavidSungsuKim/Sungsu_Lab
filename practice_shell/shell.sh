#!/bin/bash

echo "### 1. string test ##############################"
echo "who are you?:" $USER
echo "where is your home?:" $HOME

var11="Korean"
echo "what's your nationality?:"$var11

echo "### 2. calculation test #########################"
num21=100
num22=100
sum2=`expr $num21 + 200` # which is at the left of '!'
echo $sum2

echo "### 3. variable read test #######################"
echo "what is your age?:"
read age3
echo "age: " $age3

echo "### 4. parameter test ###########################"
echo "parameter1:" $0
echo "parameter2:" $1
echo "parameter3:" $2

echo "### 5. if-else test #############################"
echo "num1=" $1
echo "num2=" $2

echo "num1 = num2?"

judgement="-eq"
# a -eq b : a == b
# a -ne b : a != b
# a -gt b : a > b
# a -ge b : a >= b
# a -lt b : a < b
# a -le b : a <= b
# ! : not

if [ $1 $judgement $2 ]; then
	echo "true"
else
	echo "false"
fi

echo "### 6. case test ################################"

case "$1" in
	aaa)
		echo $1;;
	bbb)
		echo $1;;
	*)
		echo $1;;
esac

echo "Do you like me? (yes / no)"
read answer6
case $answer6 in
	yes | y | Y | Yes | YES)
		echo "yes";;
	[nN]*)
		echo "no";;
	*)
		echo "you should choose";;
esac


echo "### 7. logical expression test ##################"


echo "### 8. for test  ################################"

sum8=0
for i in 0 1 2 3 4 5 6 7 8 9
#for ((i=0;i<10;i++)) # for this to work, this script must run by bash, not sh.
#for i in $(seq 0 1 9)
do
	sum8=`expr $sum8 + $i`
	echo "i=$i, sum=$sum8"
done

for fname8 in $(ls *.test)
do
	echo "filename = $fname8"
done

echo "### 9. while test ###############################"

sum9=0
#while [ $sum9 -lt 10 ] # as long as sum9 <= 10
until [ $sum9 -ge 10 ]  # unless sum9 >= 10

#while [ 1 ]
#while [ : ] 
do
	sum9=`expr $sum9 + 1`
	echo "press 'ctrl+c' to stop printing! (i=$sum9)"
done

#Note: break and continue can also be used in while.

echo "### 10. function test ###########################"

func10 ()
{
	echo $0
	echo $1
	echo $2
}

func10 1 2 

echo "### 11. eval test ###############################"

str11="ls -l"

echo $str11
eval $str11


echo "### 12. export test #############################"

var121="local variable"
echo "$var121"
echo "$var122" # which comes from export.sh

echo "### 13. printf test #############################"

var131=10.1
var132="printf test"
printf "number=%.1f \t %s\n" $var131 "$var132"  

echo "### 14. set test ################################"

echo "Today is: $(date)"
set $(date)
echo "\$0: $0"
echo "\$1: $1"
echo "\$2: $2"
echo "\$3: $3"

echo "### 15. shift test ##############################"

func15 ()
{
	echo "$1"
	str=""
	while [ "$1" != "KKK" ] ; do
		str="$str $1"
		shift
		echo "$1"
	done
	echo $str
}

func15 AAA BBB CCC DDD EEE FFF GGG III JJJ KKK

exit 0


