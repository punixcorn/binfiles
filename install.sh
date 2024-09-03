#!/bin/bash

function error() {
    printf "\e[31mERR: \e[0m%s\n" "$1"
    exit 1
}

function Installcompletion() {
    printf "%s\n%s\n" "# binfiles completion setup" "export PATH=\$PATH:$(pwd)" >>"$1" || error "[-] Could not write into shell startup file"
    printf "%s\n" "source $(pwd)/.completions/_bin_completions.sh >/dev/null 2>&1" >>"$1" || error "[-] could not write in to shell startup file"
    printf "\e[2m[+] Done restart your shell for completion and binaries to take effect\e[0m\n"
}

echo "Installing binfiles configs"

if [ -f "$HOME/.zshrc" ]; then
    if [ -f /bin/apt ]; then
        printf "%s\n%s\n%s\n" "# fix complete not found in debain" "autoload -U +X bashcompinit && bashcompinit" "autoload -U +X compinit && compinit" >>"$HOME/.zshrc"
    fi
    printf "\e[33m%s\e[0m\n" "[+] zsh shell found\n"
    Installcompletion "$HOME/.zshrc"
elif [ -f "$HOME/.bashrc" ]; then
    printf "\e[33m%s\e[0m\n" "[+] bash shell found\n"
    Installcompletion "$HOME/.bashrc"
else
    error "[-] Could not find any shell startup files : .bashrc .zshrc\n"
fi
