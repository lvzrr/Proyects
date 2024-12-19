#!/bin/bash
cd ~/dev/clearf/ || return
echo "Getting Keyboard Device Stream..."
device=$(more /proc/bus/input/devices | grep led)
arr=($device)

checkheader=$(cat ~/dev/clearf/clear.h | grep "#define SOCKET")

if [ "$checkheader" != "" ]; then
    echo "Removing line  $checkheader"
    sed -i '/#define SOCKET/d' ~/dev/clearf/clear.h
fi
for i in "${arr[@]}"; do
    if [[ "$i" == event* ]]; then
        echo "Keyboard Found At: /dev/input/$i"
        echo "Inyecting Macro..."
        echo "#define SOCKET \"/dev/input/$i\"" >>~/dev/clearf/clear.h
    fi
done
echo "Recompiling..."
gcc clear.c -o clearf -lraylib -O3 && chmod u+s ./clearf && eval "./clearf &"
