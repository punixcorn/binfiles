#/usr/bin/env bash
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

_helper_completion() {
	COMPREPLY+=($(compgen -W " -h " "${COMP_WORDS[1]}" 2>/dev/null))
}

_gitalias_completions() {
	COMPREPLY+=($(
		compgen -W " --help -h --message -m --commit -c --add -a --init -i --branch -b  -s --switch -d --delete -M --Merge --Pull -P --push -p --Clone -C --Request -R --verbose -v --log -l --Status -S --origin -o --repo -r --Des -D --type -t --undo -u --git -g --Grab -G" "${COMP_WORDS[1]}" 2>/dev/null
	))
}

_makehere_completions() {
	COMPREPLY+=($(compgen -W "-h --help  -l  --language -c --compiler -f --filename -std --standard  -s --skip" "${COMP_WORDS[1]}" 2>/dev/null))
}

_msd_completions() {
	COMPREPLY+=($(compgen -W "-h -lsblk" "${COMP_WORDS[1]}" 2>/dev/null))
}

_setHdmi_completions() {
	COMPREPLY+=($(compgen -W "-h -m -s --show --off -off --help " "${COMP_WORDS[1]}" 2>/dev/null))
}

complete -F _gitalias_completions gitalias
complete -F _makehere_completions makehere
complete -F _msd_completions msd
complete -F _msd_completions umsd
complete -F _helper_completion runmain
complete -F _setHdmi_completions setHdmi
