#!/usr/bin/python

class Exception1(Exception):
    def __init__(self, errCode, errMsg):
        self.errCode = errCode
        self.errMsg = errMsg
    def __str__(self):
        return "Exception1: %s(%d)" % (self.errMsg, self.errCode)

class Exception2(Exception):
    def __init__(self, errCode):
        self.errCode = errCode
    def __str__(self):
        return "Exception2: %d" % (self.errCode)

class Exception3(Exception):
    def __init__(self, errMsg):
        self.errMsg = errMsg
    def __str__(self):
        return "Exception3: %s" % (self.errMsg)

try:
    exceptionType = raw_input("Exception type(1, 2, 3): ")
    if exceptionType == "1":
        raise Exception1(100, "Access denied")
    if exceptionType == "2":
        raise Exception2(100)
    if exceptionType == "3":
        raise Exception3("Access denied")
except (Exception1, Exception2) as e:
    print e
except Exception3 as e:
    print "Caught exception3: %s" % (e.errMsg)
