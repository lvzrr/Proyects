#!/bin/bash

sed -i '/# RECREATIONAL PROGRAMMING/!d' README.md

warning="> [!Warning]\n*This is a personal repo for personal use, code might be _*UNSAFE*_, not well documented or unintuitive, use at your own risk*"
echo -e "$warning" >>README.md

for readme in */README.md; do
    echo "Processing $readme"

    while IFS= read -r line; do
        if [[ "$line" == \#* ]]; then
            echo "#$line" >>README.md
        else
            echo "$line" >>README.md
        fi
    done <"$readme"
done
