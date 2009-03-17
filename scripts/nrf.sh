#!/bin/sh
# tony
# nrf = name reset from a file and copy into another path
# rename certain files list in a file to 1, 2, 3, ... with the given postfix into the given path

usage()
{
	echo "usage: `basename "$0"` list_file result_path postfix"
	echo "rename and copy certain files list in the list_file to 1, 2, 3, ... with the given postfix"
	echo "i.e. ./`basename "$0"` "\"sortedSkin.txt\"" \"test/images\" \"jpg\""
}

if [ $# -lt 3 ]; then
	usage
	exit 0
fi

i=1

cat "$1" | 
while read file
do
	echo "$file -> $2/$i.$3"
	cp "$file" "$2/$i.$3"
	i=`expr $i + 1`	
done

