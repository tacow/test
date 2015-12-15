#!/usr/bin/python
import sys
import os

if len(sys.argv) < 2:
    path = "."
else:
    path = sys.argv[1]

try:
    for file in os.listdir(path):
        filePath = os.path.join(path, file)

        if os.path.isfile(filePath):
            print "%s %d" % (filePath, os.path.getsize(filePath))

        if os.path.isdir(filePath):
            print "[%s]" % (filePath)
except OSError, e:
    print e
