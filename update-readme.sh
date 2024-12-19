#!/bin/bash
function gen_table() {
    header="# Latest Commits:\n| Commit Hash | Commit Msg | Author | Date |\n|-------------|------------|--------|------|\n| CURRENT | $1 | $2 | $3 |"

    commits=$(git log -n 10 --pretty=format:"%h|%s|%an|%as")

    echo -e "$header" >>README.md

    while IFS= read -r commit; do
        commit_hash=$(echo "$commit" | cut -d'|' -f1)
        commit_msg=$(echo "$commit" | cut -d'|' -f2)
        author=$(echo "$commit" | cut -d'|' -f3)
        date=$(echo "$commit" | cut -d'|' -f4)

        echo "| $commit_hash | $commit_msg | $author | $date |" >>README.md
    done <<<"$commits"

    echo "Table successfully added to README.md"
}

function gen_repo_README() {
    toc="## Table of contents:\n"
    header="# RECREATINAL PROGRAMMING\n> [!Warning]\n**This is a personal repo for personal use, code might be *UNSAFE*, not well documented or unintuitive, use at your own risk**"
    readme_md=""
    for readme in */README.md; do
        proyectname="${readme%/README.md}"

        proyectname="$(echo -e "$proyectname" | sed -e 's/^[[:space:]]*//g' -e 's/[[:space:]]*$//g')"

        while IFS= read -r line; do
            if [[ "$line" == \#* ]]; then
                line_no_header="${line#\# }"

                lineheader="${line%%[^#]*}"

                lineheader="$(echo -e "$lineheader" | sed -e 's/^[[:space:]]*//g' -e 's/[[:space:]]*$//g')"

                if [[ "$line_no_header" == "$proyectname" ]]; then
                    readme_md+="${lineheader}# [$proyectname](https://github.com/lvzrr/Recreational-Programming/tree/main/$proyectname)\n" >>README.md
                    toc+="- [$proyectname](#$proyectname)\n"
                else
                    readme_md+="#$line\n" >>README.md
                fi
            else
                readme_md+="$line\n" >>README.md
            fi
        done <"$readme"
        echo "[+] $readme"
    done
    echo -e "$header\n$toc\n$readme_md" >>README.md
    echo -e "\n\n"
}

echo -e -n "1. Update README.md files\n2. Commit all changes\n3. Dont push changes\n\nSelect an option: "

read -r option
echo "" >README.md

case "$option" in
1) gen_repo_README && gen_table "Update README.md" "$(git config user.name)" "$(date "+%Y-%m-%d")" && git add README.md && git commit -m "Update README.md" && git push ;;
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

    username=$(git config user.name)

    gen_table "$commitmsg" "$username" "$(date "+%Y-%m-%d")"
    gen_repo_README
    git add . && git commit -m "$commitmsg" && git push
    ;;
3) echo "Updates finished" ;;
*) echo "error" ;;
esac
