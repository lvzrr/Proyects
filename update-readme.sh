#!/bin/bash

echo -e "1. Commit all changes\n2. Only update the readme"

read option

sed -i '/# RECREATIONAL PROGRAMMING/!d' README.md

warning="> [!Warning]\n**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**"
echo -e "$warning" >>README.md

for readme in */README.md; do
    echo "Processing $readme"

    proyectname="${readme%/README.md}"

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

case "$option" in
1) git add README.md && git commit -m "Update README.md" && git push ;;
2) git add . && git commit -m "scripted commit" && git push ;;
esac
