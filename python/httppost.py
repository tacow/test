#!/usr/bin/python
import sys
import urllib2

if len(sys.argv) < 2:
    print "Usage: %s [url] (data)" % sys.argv[0]
    sys.exit()
url = sys.argv[1]
data = None
if len(sys.argv) >= 3:
    data = sys.argv[2]

try:
    if data:
        request = urllib2.Request(url)
        request.add_header("Content-Type", "text/plain")
        response = urllib2.urlopen(url = request, data = data, timeout = 10)
    else:
        response = urllib2.urlopen(url = url, timeout = 10)
    print response.code, response.msg
    print response.read()
    response.close()
except Exception as e:
    print "Exception:", e

