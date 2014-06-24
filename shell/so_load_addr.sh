#!/bin/sh

cd /usr/lib64

for sofile in *.so*
do
    readelf -l ${sofile} > /dev/null 2>&1
    if [ $? -ne 0 ]
    then
        continue
    fi
    echo -n "${sofile} "
    readelf -l ${sofile} | grep -m 1 LOAD | awk '{print $3}'
done | sort -k 2
