#!/bin/sh
# tony
# cpf = copy certain files list in a file
# copy certain files list in a file into the given path

usage()
{
	echo "usage: `basename "$0"` list_file result_path"
	echo "copy certain files list in the list_file into the given path"
	echo "i.e. ./nr.sh "\"sortedSkin.txt\"" \"test/images\""
}

if [ $# -lt 2 ]; then
	usage
	exit 0
fi

cat "$1" | 
while read file
do
	echo "$file -> $2/`basename "$file"`"
	cp "$file" "$2/`basename "$file"`"
done

