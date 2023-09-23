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
echo "make gitalias? dependencies: boost [Y,n]"
read ans
if [ "$ans" = "n" ] || [ "$ans" = "N" ]; then
	echo "not compiling gitalias"
else
	[ -d gitalias ] && cd gitalias && make || exit 1
fi

echo -e "Done"
