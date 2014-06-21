#!/bin/sh

a=0
while [ $a -lt 100 ];
do
	find . -name 'hoge' -print
	a=`expr $a + 1`
done
