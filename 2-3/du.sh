#!/bin/sh

a=0
while [ $a -lt 100 ];
do
	du -a |grep hoge
	a=`expr $a + 1`
done
