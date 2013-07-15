#!/usr/bin/python

import re

filenames = ["110000.12Q3.txt", "110000.12Q4.tmc", "310000.11Q2.ltf"]
for fName in filenames:
    res = re.match("^(\d{6})\.(\d{2})Q(\d)\.(.+)", fName)
    if res:
        print "City code: %s, year: %s, season: %s, ext: %s" % (res.group(1), res.group(2),res.group(3), res.group(4))

