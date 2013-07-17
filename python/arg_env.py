#!/usr/bin/python
import sys
import os

i = 0
for arg in sys.argv:
    print "%d: %s" % (i, arg)
    i += 1

print "#" * 20

i = 0
for var in os.environ:
    print "%d: %s = %s" % (i, var, os.environ[var])
    i += 1

