#!/bin/sh

trap IntProc INT

IntProc() {
    echo
    echo "Haha..."
    echo
}

i=1
while true
do
   echo "Press Ctrl+C... ($i)"
   sleep 1
   ((i = i + 1))
done 
