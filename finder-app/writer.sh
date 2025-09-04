#!/bin/sh
# File: writer.sh

if [ $# -lt 2 ]
then
    echo "Usage: ./writer.sh (writefile) (writestr)"
    exit 1
fi

writefile=$1
writestr=$2

mkdir -p $(dirname ${writefile})
echo "${writestr}" > ${writefile}
