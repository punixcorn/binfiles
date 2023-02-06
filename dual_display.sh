#!/bin/bash
inFo=$( xrandr --listactivemonitors )
disPlay=$( xrandr --listactivemonitors | grep "Monitors:" | awk '{ print $2 }');

if [ $disPlay -eq 1 ];then
    printf "ERR: %s\n%s\n" "only one disPlay found" "$inFo"
    exit 1;
fi

if [ $disPlay -eq 2 ];then
    printf "%s\n" "eDP1 is always primary and on left, HDMI1 is on right and always secondary"
    xrandr --output eDP1 --primary --mode 1920x1080 --rotate normal --output HDMI1 --mode 1920x1080 --rotate normal --right-of eDP1
fi
