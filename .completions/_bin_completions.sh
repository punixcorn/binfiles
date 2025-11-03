#!/usr/bin/env bash
# a simple auto completion will work on it later

#COMP_WORDS: an array of all options
#COMP_CWORD : an index in COMP_WORDS
#COMP_LINE : the current command line
#COMPREPLY = a var used to store completions

#/usr/bin/env bash
# a simple auto completion will work on it later

#COMP_WORDS: an array of all options
#COMP_CWORD : an index in COMP_WORDS
#COMP_LINE : the current command line
#COMPREPLY = a var used to store completions


__gitalias_completions() {
    if [ -d .git ]; then
        local curr_branch=$(git rev-parse --abbrev-ref HEAD 2>/dev/null)
        local branches=$(git branch 2>/dev/null | sed -e "s/\n//g" -e "s/ //g" -e "s/*//g" 2>/dev/null)
        local other_branches=$(git branch 2>/dev/null | sed -e "s/\n//g" -e "s/ //g" -e "s/\*.*//g" 2>/dev/null)
        local commit_hashes=$(git rev-list --max-count=5 --all)
    fi
    local ls_dir=$(ls)
    # these are hash maps that dont need shell completions
    declare -A ga_comp_maps=(
        ["--commit"]="-m --message"
        ["-c"]="-m --message"
        ["--type"]="true false"
        ["-t"]="true false"
        ["--undo"]="hard mixed soft"
        ["-u"]="hard mixed soft"
        ["hard"]="$commit_hashes"
        ["soft"]="$commit_hashes"
        ["mixed"]="$commit_hashes"
        ["--branch"]="$branches"
        ["-b"]="$branches"
        ["-d"]="$branches"
        ["--delete"]="$branches"
        ["--switch"]="$branches"
        ["-s"]="$branches"
        ["--Merge"]="$other_branches"
        ["-M"]="$other_branches"
        ["--Visibility"]="private public"
        ["-V"]="private public"
        ["--Request"]="https ssh"
        ["-R"]="https ssh"
        ["--add"]="$ls_dir ."
        ["-a"]="$ls_dir ."
    )
    # these are hash maps that need shell' aid in completion
    declare -A ga_comp_maps_sh=(
    )

    COMPREPLY=()
    local commands="--help -h --message -m --commit -c --add -a --init -i --branch -b  -s --switch -d --delete -M --Merge --Pull -P --push -p --Clone -C --Request -R --verbose -v --log -l --Status -S --origin -o --repo -r --Des -D --type -t --undo -u --Grab -G  --Visibility -V"

    # current is compwords[i-1];
    local curr=${COMP_WORDS[$((COMP_CWORD - 1))]}
    trip=0
    local save_curr=""

    echo
    if [ $COMP_CWORD -eq 1 ]; then
        COMPREPLY=($(compgen -W "$commands" -- "${COMP_WORDS[1]}"))
    elif [[ -n "${ga_comp_maps[$curr]}" ]]; then
        save_curr=$curr
        trip=1
        COMPREPLY=($(compgen -W "${ga_comp_maps[$curr]}" -- "$curr"))
    else
        COMPREPLY=($(compgen -W "$commands" -- "${COMP_WORDS[1]}"))
    fi

}

_helper_completion() {
    COMPREPLY+=($(compgen -W " -h " "${COMP_WORDS[1]}" 2>/dev/null))
}

_cstrap_completions() {
    declare -A cs_maps=(
        ["--language"]="c cpp cxx cc"
        ["-l"]="c cpp cxx cc"
        ["--compiler"]="cmake make"
        ["-c"]="cmake make"
        ["--standard"]="11 14 17 2x 2a 23"
        ["-s"]="11 14 17 2x 2a 23"
    )
    # current is compwords[i-1];
    local curr=${COMP_WORDS[$((COMP_CWORD - 1))]}
    local commands="-h --help  -l  --language -c --compiler -f --filename -std --standard  -t --template"

    echo
    if [ $COMP_CWORD -eq 1 ]; then
        COMPREPLY=($(compgen -W "$commands" -- "${COMP_WORDS[1]}"))
    elif [[ -n "${cs_maps[$curr]}" ]]; then
        COMPREPLY=($(compgen -W "${cs_maps[$curr]}" -- "$curr"))
    else
        COMPREPLY=($(compgen -W "$commands" -- "${COMP_WORDS[1]}"))
    fi
}
_msd_completions() {
    local devices=$(lsblk --all --list --paths | awk ' { print $1 } ')
    local mounted=$(lsblk --all --list --paths --noheadings | awk ' { print $7 } ' | sed -e 's/^\/$//g' | grep -vE "boot|SWAP" | sed 's/\n//g')

    # current is compwords[i-1];
    declare -A msd_maps=(
        ["--device"]="$devices"
        ["-d"]="$devices"
        ["--umount"]="$mounted"
        ["-u"]="$mounted"
    )
    local curr=${COMP_WORDS[$((COMP_CWORD - 1))]}
    local commands="-h --help -lsblk -u --umount --help -p --place -d --device"
    if [ $COMP_CWORD -eq 1 ]; then
        COMPREPLY=($(compgen -W "$commands $devices" -- "${COMP_WORDS[1]}"))
    elif [[ -n "${msd_maps[$curr]}" ]]; then
        COMPREPLY=($(compgen -W "${msd_maps[$curr]}" -- "$curr"))
    else
        COMPREPLY=($(compgen -W "$commands $devices" -- "${COMP_WORDS[1]}"))
    fi
}

_setHdmi_completions() {
    COMPREPLY+=($(compgen -W "-h --help -m -M --Mainszie -H --Hdmisize -s --show -p --place -o --off " "${COMP_WORDS[1]}" 2>/dev/null))
}

_ff_completions() {
    COMPREPLY+=($(compgen -W "-h --help --parms -p -v " "${COMP_WORDS[1]}" 2>/dev/null))
}
_makezip_completions() {
    COMPREPLY+=($(compgen -W "-h --help -t --targets -c --changes" "${COMP_WORDS[1]}" 2>/dev/null))
}

complete -F __gitalias_completions gitalias
complete -F _cstrap_completions cstrap
complete -F _msd_completions msd
complete -F _helper_completion runmain
complete -F _setHdmi_completions setHdmi
complete -F _ff_completions ff
complete -F _makezip_completions makezip
