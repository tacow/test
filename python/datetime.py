#!/usr/bin/python
import time

ttNow = int(time.time())
stNow = time.localtime(ttNow)
strNow = time.strftime("%Y-%m-%d %H:%M:%S", stNow)
print "UNIX timestamp:", ttNow
print "Date time:", strNow
