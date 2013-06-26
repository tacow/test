#!/usr/bin/python
import sys
import httplib

if len(sys.argv) < 2:
    print "Usage: %s [host] [path]" % sys.argv[0]
    sys.exit()
host = sys.argv[1]
if len(sys.argv) < 3:
    path = "/"
else:
    path = sys.argv[2]

conn = httplib.HTTPConnection(host)
conn.request("GET", path)
response = conn.getresponse()
print response.status, response.reason
print response.read()
conn.close()

