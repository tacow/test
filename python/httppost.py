#!/usr/bin/python
import sys
import urllib2

if len(sys.argv) < 3:
    print "Usage: %s [url] [message]" % sys.argv[0]
    sys.exit()
url = sys.argv[1]
message = sys.argv[2]

try:
    request = urllib2.Request(url)
    request.add_header("Content-Type", "text/plain")
    response = urllib2.urlopen(url = request, data = message, timeout = 10)
    print response.code, response.msg
    print response.read()
    response.close()
except Exception as e:
    print "Exception:", e

