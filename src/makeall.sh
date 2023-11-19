#!/bin/bash

col1="\e[32m"
col2="\e[33m"
rs="\e[0m"

DIR=$1
[ -z "$DIR" ] && DIR='bin'

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
findexec "cmake"

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

echo "dependencies: boost & fmt [Y,N,s]"
read ans
if [ "$ans" = "n" ] || [ "$ans" = "N" ]; then
	echo "not compiling gitalias and makehere"
elif [ "$ans" = "y" ] || [ "$ans" = "Y" ]; then
	extra_packages=""
	[ ! -f /bin/cmake ] && echo "you need cmake too...." && extra_packages=" cmake"
	[ -f /bin/apt ] && sudo apt install libboost-all-dev $extra_packages libfmt-dev
	[ -f /bin/pacman ] && sudo pacman -S boost-libs boost fmt $extra_packages

	# build
	cmake -S . -B build/
	cmake --build build/
else
	cmake -S . -B build/
	cmake --build build/
fi

rm -rf build/

echo -e "Done"
echo -e "binaries in bin"
