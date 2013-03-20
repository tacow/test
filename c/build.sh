#!/bin/sh

build() {
gcc -g -o args args.c
}

clean() {
rm args
}

if [ "$1" = "" ]
then
    build
fi

if [ "$1" = "clean" ]
then
    clean
fi

