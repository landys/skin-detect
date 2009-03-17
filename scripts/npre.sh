#!/bin/bash
# tony
# npre = add prefix to all file name

usage()
{
	echo "usage: `basename "$0"` path expression(no space) prefix"
	echo "add prefix to all file name"
	echo "i.e. ./`basename "$0"` \"/cygdrive/e/codes/ForFlex201/FlexDemo/images\" \"*.jpg\" \"N\""
}

if [ $# -lt 3 ]; then
	usage
	exit 0
fi

cd "$1"

# if file name has " ", then wrong
# for a file "gloomy sunday.jpg", which will be treated as two files "gloomy" and "sunday.jpg"
#for file in `find . -name "$2" -type f`
#ls "../a/*.txt" # wrong
#ls ../a/*.txt	# right, but the path cannot has space(" ")
#ls -A $2 |	# OK, but feel not so robust, if no $2, will an error
find . -name "$2" -type f | 
while read file
do
	#echo "$file"
	#echo `dirname "$file"`"/""${3}"`basename "$file"`
	mv "$file" `dirname "$file"`"/""${3}"`basename "$file"`
done
