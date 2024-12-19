#!/bin/bash
cd ~/dev/KeyWatcher/ || return
echo "Getting Keyboard Device Stream..."
device=$(more /proc/bus/input/devices | grep led)
arr=($device)

checkheader=$(cmd <~/dev/KeyWatcher/keys.h | grep event)
if [ "$checkheader" != "" ]; then
    sed -i "/$checkheader/d" ~/dev/KeyWatcher/keys.h
fi

for i in "${arr[@]}"; do
    if [[ "$i" == event* ]]; then
        echo "Keyboard Found At: /dev/input/$i"
        echo "Inyecting Macro..."
        echo "#define SOCKET \"/dev/input/$i\"" >>~/dev/KeyWatcher/keys.h
    fi
done
echo "Recompiling..."
gcc key.c -o keys -lraylib -O3 && chmod u+s ./keys && eval "~/dev/KeyWatcher/keys &"
return
