#!/bin/bash

#USER=`whoami`
#if [ $USER != 'root' ];
#then
#echo You must be root to run this script!
#exit
#fi

# Fade Ad
COUNTDOWN=9
YELO=FFFF00
GREE=00FF00
AD="\n\c6  ScreenDUO4macOS\c4\b25,275,197,198,\b25,26,197,219,\b25,275,219,220,\b275,276,197,219,"

while [ 1 ]
do
TODAY=`date`
LOAD=`ps -A -o %cpu | awk '{s+=$1} END {print s "%"}'`
#TOPROC=`ps aux --sort -c | head -2 | tail -1 | awk '{print $11}' | sed 's,^.*/,,' | cut -c 1-8`
#TOPROCID="`ps aux --sort -c | head -2 | tail -1 | awk '{print $2" "$3}'`"
#FREEMEM=`top -n 1 | head -4 | tail -1 | awk '{print $6}'`
#CPU=`sensors | grep "CPU T" | awk '{print $3}'`
#CPUT=`sensors | grep "CPU T" | cut -d' ' -f6-8 | sed 's,+,,' | sed 's,\.[0-9].*,,'`
#if [ $CPUT -lt 40 ]
#then
#COLOR=00FFFF
#LENGTH=50
#fi
#if [ $CPUT -gt 40 ]
#then
#COLOR=40FF00
#LENGTH=100
#fi
#if [ $CPUT -gt 45 ]
#then
#COLOR=FFFF00
#LENGTH=150
#fi
#if [ $CPUT -gt 50 ]
#then
#COLOR=FFBF00
#LENGTH=200
#fi
#if [ $CPUT -gt 55 ]
#then
#COLOR=FF8000
#LENGTH=250
#fi
#if [ $CPUT -gt 60 ]
#then
#COLOR=FF0000
#LENGTH=300
#fi
#FANLENGTH=`sensors | grep IS2 | awk '{print $4}'`
#FANLENGTH=`expr $FANLENGTH \- 910`
#FANBAR="\c7\b0,$FANLENGTH,56,57,"
#CPUBAR="\a$COLOR\b5,$LENGTH,160,162,"
#RPMS=`sensors | grep FAN | awk '{print $4}'`
BANNER="\c12\c20\c31\c46 \c5M\c6a\c7y\c1o\c2r \c3B\c4o\c5b\c6s\c7t\c1e\c2r"
AD="\n\a$YELO  ScreenDUO4macOS"
#./duo "\c5`date` \caload:$LOAD\n\c4RPMS:$RPMS$FANBAR\n\c3MEMFREE: $FREEMEM k\n\c7PID  %cpu Name\n\c6$TOPROCID $TOPROC\n\c2CPU $CPU\n$CPUBAR\n$AD"
./duo "\c5`date` \n\caCPU: $LOAD\n\n$AD"
if [ $COUNTDOWN -gt 0 ]
then
COUNTDOWN=`expr $COUNTDOWN - 1`
if [ $COUNTDOWN -lt 8 ];
then
YELO=EEEE00
GREE=00EE00
fi
if [ $COUNTDOWN -lt 7 ];
then
YELO=AAAA00
GREE=00AA00
fi
if [ $COUNTDOWN -lt 6 ];
then 
YELO=777700
GREE=007700
fi
if [ $COUNTDOWN -lt 5 ];
then
YELO=444400
GREE=004400
fi
if [ $COUNTDOWN -lt 4 ];
then
YELO=111100
GREE=001100
fi
if [ $COUNTDOWN -lt 3 ];
then
YELO=444400
GREE=004400
fi
if [ $COUNTDOWN -lt 2 ];
then 
YELO=777700
GREE=007700
fi
if [ $COUNTDOWN -lt 1 ];
then
YELO=AAAA00
GREE=00AA00
COUNTDOWN=8
fi
fi
sleep 0.5
done
