#!/bin/bash 


function error(){
    printf "\e[31mERR: \e[0m%s\n" "$1"
    exit 1 
}

function runIn (){
    printf "%s\n%s\n" "# binfiles" "export PATH=\$PATH:\$HOME/binfiles" >> "$1" || error "Could not write into shell startup file"

    printf "%s\n\t%s\n%s\n" "for filesX in \$(ls \$HOME/binfiles/.completions);do" "source \$HOME/binfiles/.completions/\$filesX >/dev/null 2>&1" "done">> "$1" || error "could not write in to shell startup file"
    printf "\e[2mDone\e[0m"
}

if [ -f "$HOME/.zshrc" ];then
    printf "\e[33m%s\e[0m\n" "zsh shell found" 
    runIn "$HOME/.zshrc"
elif [ -f "$HOME/.bashrc" ]; then
    printf "\e[33m%s\e[0m\n" "bash shell found" 
    runIn "$HOME/.bashrc"
else 
    error "Could not find any shell startup files : .bashrc .zshrc ";
fi
    

