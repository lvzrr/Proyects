#!/bin/bash

sed -i '/# RECREATIONAL PROGRAMMING/!d' README.md

warning="> [!Warning]\n**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**"
echo -e "$warning" >>README.md

for readme in */README.md; do
    echo "Processing $readme"

    proyectname="${readme%/README.md}" # This removes "/README.md" from the path to get the project name

    proyectname="$(echo -e "$proyectname" | sed -e 's/^[[:space:]]*//g' -e 's/[[:space:]]*$//g')"

    while IFS= read -r line; do
        if [[ "$line" == \#* ]]; then
            line_no_header="${line#\# }"

            lineheader="${line%%[^#]*}"

            lineheader="$(echo -e "$lineheader" | sed -e 's/^[[:space:]]*//g' -e 's/[[:space:]]*$//g')"

            if [[ "$line_no_header" == "$proyectname" ]]; then
                echo -e "${lineheader}# [$proyectname](https://github.com/lvzrr/Recreational-Programming/tree/main/$proyectname)" >>README.md
            else
                echo -e "#$line" >>README.md
            fi
        else
            echo -e "$line" >>README.md
        fi
    done <"$readme"
done
