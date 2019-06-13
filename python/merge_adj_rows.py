#!/usr/bin/python
from __future__ import print_function
import sys

if len(sys.argv) < 2:
    print("Usage: %s [filename]" % sys.argv[0])
    sys.exit()
filename = sys.argv[1]

f = open(filename)
lineNum = 0
for line in f:
    line = line.rstrip("\r\n")
    if lineNum % 2 == 0:
        print("%s" % line, end = "")
    else:
        print("%s" % line)
    lineNum += 1

f.close()

