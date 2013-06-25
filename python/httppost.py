#!/usr/bin/python
import sys
import urllib
import httplib

if len(sys.argv) < 3:
    print "Usage: %s [host] [message]" % sys.argv[0]
    sys.exit()
host = sys.argv[1]
message = sys.argv[2]

params = urllib.urlencode({"message": message})
headers = {"Content-Type": "application/x-www-form-urlencoded"}
conn = httplib.HTTPConnection(host)
conn.request(method = "POST", url = "/", body = params, headers = headers)
response = conn.getresponse()
print response.status, response.reason
print response.read()
conn.close()

