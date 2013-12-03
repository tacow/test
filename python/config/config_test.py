#!/usr/bin/python

from config import ReadConfig

keyVals = ReadConfig("test.conf", "=")
for key in keyVals:
    print key, keyVals[key]

