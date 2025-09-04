#!/bin/sh
# File: finder.sh

if [ $# -lt 2 ]
then
    echo "Usage: ./finder.sh (filesdir) (searchstr)"
    exit 1
elif [ ! -d "$1" ]
then
    echo "Error: $1 is not a directory"
    exit 1
fi

filesdir=$1
searchstr=$2

echo -n "The number of files are "
echo -n "$(($(find ${filesdir} | wc -l)-1))"
echo -n " and the number of matching lines are "
echo "$(grep -l -r ${searchstr} ${filesdir}/* | wc -l)"
