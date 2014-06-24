#!/bin/sh

TEMP_FILE=/tmp/so_load_addr.tmp
LIB_PATH=/usr/lib64

cd ${LIB_PATH}

for sofile in *.so*
do
    readelf -l ${sofile} > ${TEMP_FILE} 2> /dev/null
    if [ $? -ne 0 ]
    then
        continue
    fi
    echo -n "${sofile} "
    grep -m 1 LOAD ${TEMP_FILE} | awk '{print $3}'
done | sort -k 2
