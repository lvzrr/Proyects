#!/bin/bash

sed -i '/# RECREATIONAL PROGRAMMING/!d' README.md

warning="> [!Warning]\n**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**"
echo -e "$warning" >>README.md

for readme in */README.md; do
    echo "Processing $readme"

    proyectname="${readme%/README.md}" # This removes "/README.md" from the path to get the project name

    while IFS= read -r line; do
        if [[ "$line" == \#* ]]; then
            echo -e "#$line \t[🔗$proyectname](https://github.com/lvzrr/Recreational-Programming/tree/main/$proyectname)" >>README.md
        else
            echo -e "$line" >>README.md
        fi
    done <"$readme"
done
