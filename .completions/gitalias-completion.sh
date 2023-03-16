#/usr/bin/env bash
# a simple auto completion will work on it later

#COMP_WORDS: an array of all options 
#COMP_CWORD : an index in COMP_WORDS 
#COMP_LINE : the current command line 
#COMPREPLY = a var used to store completions 

_gitalias_completions(){
    COMPREPLY+=($(compgen -W " --help -h --message -m --commit -c --add -a --init -i --branch -b  -s --switch -d --delete -M --Merge --Pull -P --push -p --Clone -C --Request -R --verbose -v --log -l --Status -S --origin -o --repo -r --Des -D --type -t --undo -u --git -g" "${COMP_WORDS[1]}" 2>/dev/null))

}

complete -F _gitalias_completions gitalias
