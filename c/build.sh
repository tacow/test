#!/bin/sh

build() {
gcc -g -o args args.c
gcc -g -o client client.c
}

clean() {
rm args
rm client
}

if [ "$1" = "" ]
then
    build
fi

if [ "$1" = "clean" ]
then
    clean
fi

