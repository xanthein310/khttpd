#!/bin/bash

#	./fibcheck.sh $1 $2 $3
#	$1: order of number  $2 type 
#	$3 ans
#
ORDER="$1"
TYPE="$2"
DATA="$3"
ANS=""

function print_usage()
{
	echo	"[Usage] ./fibcheck.sh \$1 \$2 \$3"
	echo	"\$1: order of number"  
	echo	"\$2: type" 
	echo	"\$3: ans"
	exit 1
}
function case_sw() 
{
	case $TYPE in
	'2')
		ANS=`grep -A 1 "Binary Base 2" $ORDER.log|tail -1`
		;;
	'3')
		ANS=`grep -A 1 "Ternary Base 3" $ORDER.log|tail -1`
		;;
	'5')
		ANS=`grep -A 1 "Quinary Base 5" $ORDER.log|tail -1`
		;;
	'6')
		ANS=`grep -A 1 "Senary Base 6" $ORDER.log|tail -1`
		;;  
	'8')
		ANS=`grep -A 1 "Octal Base 8" $ORDER.log|tail -1`
		;;  
	'10')
		ANS=`grep -A 1 "Decimal Base 10" $ORDER.log|tail -1`
		;; 
	'16')
		ANS=`grep -A 1 "Hexadecimal Radix 16" $ORDER.log|tail -1`
		;;
	'36')
		ANS=`grep -A 1 "Hexatridecimal Radix 36" $ORDER.log|tail -1`
		;;
	'63404')
		ANS=`grep -A 1 "Radix 63404" $ORDER.log|tail -1`
		;;
	*)
		echo "TYPE only 2/ 3/ 5/ 6/ 8/ 10/ 16/ 36/ 63404/"
		exit 1;
		;;
	esac
	echo $ANS
}
do_main()
{
	curl -s http://www.protocol5.com/Fibonacci/$ORDER.htm | html2text > $ORDER.log
	case_sw
	[ $ANS == $DATA ] && echo "1" || echo "0" 
	rm -f $ORDER.log
}

[ "$#" -eq "0" ] && print_usage
do_main
