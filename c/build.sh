#!/bin/sh

PROGS=" args \
        system \
        client \
        iconv \
        server \
        strftime\
        rand"

compile() {
    PROG=$1
    gcc -g -o ${PROG} ${PROG}.c
}

build() {
    for PROG in ${PROGS}
    do
        echo "Compiling ${PROG}..."
        compile ${PROG}
    done
}

clean() {
    for PROG in ${PROGS}
    do
        rm -f ${PROG}
    done
}

if [ "$1" = "" ]
then
    build
elif [ "$1" = "clean" ]
then
    clean
else
    compile "$1"
fi

