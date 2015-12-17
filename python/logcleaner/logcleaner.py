#!/usr/bin/python
import sys
import os
import re
import time

LOG_POSTFIX = "_(\d\d\d\d\d\d\d\d\d\d\d\d\d\d).log"

class FileInfo:
    def __init__(self):
        self.name = None
        self.path = None
        self.size = 0
        self.time = 0

    def __str__(self):
        return "File name: %s, path: %s, size: %d, time: %d" % (self.name, self.path, self.size, self.time)

    def __repr__(self):
        return "{ %s }" % str(self)


class PrefixInfo:
    def __init__(self):
        self.config = None
        self.curFile = None
        self.hisFiles = []

    def __str__(self):
        if self.config:
            s = "Prefix: %s\n" % self.config[0]
        if self.curFile:
            s += "Current log file:\n    %s: %d\n" % (self.curFile.name, self.curFile.size)
        hisFileNames = "\n    ".join("%s: %d" % (hisFile.name, hisFile.size) for hisFile in self.hisFiles)
        s += "History log files:\n    %s\n" % hisFileNames
        return s

    def __repr__(self):
        return "{\n%s}" % str(self)

def LoadConfig(confFile):
    config = []
    f = open(confFile)
    for line in f:
        line = line.strip()
        if line.startswith("#"):
            continue
        tokens = re.split("\s+", line)
        if len(tokens) < 2:
            print "Invalid config line: %s" % line
        prefix = tokens.pop(0)
        megabytes = 0
        hours = 0
        for token in tokens:
            res = re.match("(\d+)(M|G|H|D)", token)
            if res:
                num = int(res.group(1))
                unit = res.group(2)
                if unit == "M":
                    megabytes = num
                elif unit == "G":
                    megabytes = num * 1024
                elif unit == "H":
                    hours = num
                elif unit == "D":
                    hours = num * 24
            else:
                print "Invalid parameter: %s" % token
        config.append((prefix, megabytes, hours))
    return config

def CleanLog(logPath, config):
    fileInfos = []
    for fileName in os.listdir(logPath):
        filePath = os.path.join(logPath, fileName)
        if not os.path.isfile(filePath):
            continue
        fileInfo = FileInfo()
        fileInfo.name = fileName
        fileInfo.path = filePath
        fileInfo.size = os.path.getsize(filePath)
        fileInfos.append(fileInfo)

    print "Log statistics:"
    print "=" * 50

    firstPrefix = True
    prefixInfos = []
    for prefixConfig in config:
        prefix = prefixConfig[0]
        hisFilePattern = prefix + LOG_POSTFIX

        prefixInfo = PrefixInfo()
        prefixInfo.config = prefixConfig
        prefixInfo.curFile = None
        prefixInfo.hisFiles = []
        for fileInfo in fileInfos:
            if fileInfo.name == prefix + ".log":
                prefixInfo.curFile = fileInfo
                continue
            res = re.match(hisFilePattern, fileInfo.name)
            if res:
                fileInfo.time = time.mktime(time.strptime(res.group(1), "%Y%m%d%H%M%S"))
                prefixInfo.hisFiles.append(fileInfo)
        prefixInfo.hisFiles.sort(key = lambda hisFile: hisFile.name, reverse = True)
        prefixInfos.append(prefixInfo)

        if firstPrefix:
            firstPrefix = False
        else:
            print "-" * 50
        print str(prefixInfo),
    print

    now = time.time()
    for prefixInfo in prefixInfos:
        print "Processing \"%s\"" % prefixInfo.config[0]
        print "=" * 50

        megabytes = prefixInfo.config[1]
        hours = prefixInfo.config[2]
        exceeded = False
        totalSize = 0
        if prefixInfo.curFile:
            totalSize += prefixInfo.curFile.size

        for hisFile in prefixInfo.hisFiles:
            if not exceeded:
                totalSize += hisFile.size
                if megabytes > 0 and totalSize > megabytes * 1024 * 1024:
                    exceeded = True
                    print "Found file size exceeded"
                elif hours > 0 and now - hisFile.time > hours * 3600:
                    exceeded = True
                    print "Found file time exceeded"

            if exceeded:
                os.remove(hisFile.path)
                print "Remove %s" % hisFile.name
            else:
                print "%s pass" % hisFile.name
        print

if len(sys.argv) < 3:
    print "Usage:\n\t%s [log path] [config file]" % sys.argv[0]
    sys.exit()
logPath = sys.argv[1]
confFile = sys.argv[2]

try:
    config = LoadConfig(confFile)
    CleanLog(logPath, config)
except Exception, e:
    print e

