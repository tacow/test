#!/usr/bin/python
import sys
from urllib import urlencode

if len(sys.argv) < 2:
    print "Usage: %s [filename]" % sys.argv[0]
    sys.exit()
filename = sys.argv[1]

f = open(filename)
str = f.read()
f.close()

query = {}
query["content"] = str
print urlencode(query)

