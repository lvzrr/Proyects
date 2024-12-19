#!/bin/bash

echo -e -n "1. Update README.md files\n2. Commit all changes\n3. Dont push changes\n\nSelect an option: "

read -r option

function gen_repo_README() {

    sed -i '/# RECREATIONAL PROGRAMMING/!d' README.md

    warning="> [!Warning]\n**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**"
    echo -e "$warning" >>README.md

    for readme in */README.md; do
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
        echo "[+] $readme"
    done
    echo -e "\n\n"

}

function gen_table() {
    header="# Latest Commits:\n| Commit Hash | Commit Msg | Author |\n|--------------|----------------------------|-----------|"
    commits=$(git log -n 10 --pretty=format:"%h %s %an")

    echo -e "$header" >>README.md

    while IFS= read -r commit; do
        # Split the commit hash and message
        commit_hash=$(echo "$commit" | awk '{print $1}')
        commit_msg=$(echo "$commit" | cut -d' ' -f2-)
        author=$(echo "$commit" | awk '{print $3}')
        echo "added commit $commit_hash to table"
        echo "| $commit_hash | $commit_msg | $author |" >>README.md
    done <<<"$commits"
}

case "$option" in
1) gen_repo_README && gen_table && git add README.md && git commit -m "Update README.md" && git push ;;
2)
    diffs=$(eval "git diff")
    echo -e "DIFFS: \n$diffs\n"
    if [ "$diffs" == "" ]; then
        echo "no changes to commit"
        exit
    fi
    echo -n "Please input a commit message: "
    read -r commitmsg
    echo -e "Generating README.md for the repo...\n\n"
    gen_repo_README
    gen_table
    git add . && git commit -m "$commitmsg" && git push
    ;;
3) echo "Updates finished" ;;
*) echo "error" ;;
esac
