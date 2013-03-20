#!/bin/sh

PROGS="args \
       client \
       server"

build() {
    for PROG in ${PROGS}
    do
        gcc -g -o ${PROG} ${PROG}.c
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
fi

if [ "$1" = "clean" ]
then
    clean
fi

