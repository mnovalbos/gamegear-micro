#!/bin/sh



for FILE in $(ls _file*)
do
	cat $FILE >> assets2.bin
done
