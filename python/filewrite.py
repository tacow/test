#!/usr/bin/python
import sys

if len(sys.argv) < 2:
    print "Usage: %s [filename]" % sys.argv[0]
    sys.exit()
filename = sys.argv[1]

f = open(filename, "w")
f.write("abc\n")
f.write("def\n")
f.write("ghi\n")
f.close()

