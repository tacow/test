#!/usr/bin/python
import sys

if len(sys.argv) < 2:
    print "Usage: %s [filename]" % sys.argv[0]
    sys.exit()
filename = sys.argv[1]

f = open(filename)
str = f.read()
print str

print "#" * 20

f.seek(0)
lineNum = 1
for line in f:
    line = line.rstrip("\r\n")
    print "%d %s" % (lineNum, line)
    lineNum += 1

f.close()

