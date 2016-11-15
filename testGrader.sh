#!/bin/bash

calc=50
readcalc=70
duration=$(echo ${calc} ${readcalc} | awk '{ printf "%3f", $1 - $2 }')
zero=0
tmp=`echo $duration'>'$zero | bc -l`
echo $duration'>'$zero 
echo $tmp
if [ $tmp -eq 0 ]
then
    echo testcase ${TESTCASE} using 0 seconds
else
    echo testcase ${TESTCASE} using $calc - $readcalc = ${duration} seconds
fi