#!/bin/bash

col1="\e[32m"
col2="\e[33m"
rs="\e[0m"

DIR=$1
[ -z "$DIR" ] && DIR='binary'

[ ! -d $DIR ] && mkdir $DIR

findexec() {
	[ -f "/bin/$1" ] || {
		echo -e "${col2}$1 not found${rs}"
		exit 1
	}
}

findexec "make"
findexec "gcc"
findexec "g++"
touch tempfile

echo "====== compiling ======="
for file in $(/bin/ls | grep ".\.c"); do
	echo $file >tempfile
	extension=$(cat tempfile | sed 's/.*.cpp//g')

	if [ -z "$extension" ]; then
		tempname=$(cat tempfile | sed 's/.cpp//g')
		make cpp FILE="$file" OUTPUT=$tempname && echo -e "${col1}$tempname compiled${rs}" && mv $tempname "$DIR" || echo -e "${col2}$tempfile could not compile${rs}"
	else
		tempname=$(cat tempfile | sed 's/.c//g')
		make c FILE="$file" OUTPUT=$tempname && echo -e "${col1}$tempname compiled${rs}" && mv $tempname "$DIR" || echo -e "${col2}$tempfile could not compile${rs}"
	fi
	echo -e "======================================================="
done

make clean 2>/dev/null
rm tempfile

gitalias_make() {
	[ -d gitalias ] && cd gitalias && make
	[ -f ./gitalias/bin/gitalias ] && mv ./gitalias/bin/gitalias ./../gitalias && cd gitalias && make clean || {
		echo "gitalias binary not found, failed..."
		exit 1
	}
}

echo "make gitalias? dependencies: boost [Y,n] or install boost and make gitalias [i] ?"
read ans
if [ "$ans" = "n" ] || [ "$ans" = "N" ]; then
	echo "not compiling gitalias"
elif [ "$ans" = "i" ] || [ "$ans" = "I" ]; then
	extra_packages=""
	[ ! -f /bin/cmake ] && echo "you need cmake too...." && extra_packages=" cmake"
	echo "do you want to install boost? "
	[ -f /bin/apt ] && sudo apt install libboost-all-dev $extra_packages
	[ -f /bin/pacman ] && sudo pacman -S boost-libs boost $extra_packages
	gitalias_make
else
	gitalias_make
fi

echo -e "Done"
