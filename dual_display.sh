

disPlay=$( xrandr --listactivemonitors | grep "Monitor:" | awk '{ print $2 }');
printf "%s\n" "$disPlay"

#xrandr --output eDP1 --primary --mode 1920x1080 --rotate normal --output HDMI1 --mode 1920x1080 --rotate normal --right-of eDP1
