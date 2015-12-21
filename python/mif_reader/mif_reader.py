#!/usr/bin/python
import sys
import re

class Coor:
    def __init__(self):
        self.lon = 0
        self.lat = 0
    def __str__(self):
        return "(%s, %s)" % (self.lon, self.lat)
    def __repr__(self):
        return str(self)

class Point:
    def __init__(self):
        self.coor = Coor()
        self.attrs = []
    def __str__(self):
        attrsStr = ", ".join(self.attrs)
        return "[Point %s | attributes: %s]" % (self.coor, attrsStr)
    def __repr__(self):
        return str(self)

class Line:
    def __init__(self):
        self.coor1 = Coor()
        self.coor2 = Coor()
        self.attrs = []
    def __str__(self):
        attrsStr = ", ".join(self.attrs)
        return "[Line %s - %s | attributes: %s]" % (self.coor1, self.coor2, attrsStr)
    def __repr__(self):
        return str(self)

class Pline:
    def __init__(self):
        self.coors = []
        self.attrs = []
    def __str__(self):
        coorsStr = ", ".join(str(coor) for coor in self.coors)
        attrsStr = ", ".join(self.attrs)
        return "[Pline %s | attributes: %s]" % (coorsStr, attrsStr)
    def __repr__(self):
        return str(self)

class MifInfo:
    def __init__(self):
        self.attrNames = []

        self.points = []
        self.lines = []
        self.plines = []
    def __str__(self):
        s = "Attributes\n"
        s += "-" * 50
        s += "\n"
        s += "\n".join(self.attrNames)
        if self.points:
            s += "\n\nPoints\n"
            s += "-" * 50
            s += "\n"
            s += "\n".join(str(point) for point in self.points)
        if self.lines:
            s += "\n\nLines\n"
            s += "-" * 50
            s += "\n"
            s += "\n".join(str(line) for line in self.lines)
        if self.plines:
            s += "\n\nPlines\n"
            s += "-" * 50
            s += "\n"
            s += "\n".join(str(pline) for pline in self.plines)
        return s
    def __repr__(self):
        return str(self)

def ReadAttrs(midF):
    attrs = []
    line = midF.readline()
    line = line.strip()
    tokens = line.split(",")
    for token in tokens:
        res = re.match("\"(.*)\"", token)
        if res:
            attr = res.group(1)
        else:
            attr = token
        attrs.append(attr)
    return attrs
    
def ReadMif(mifFile, midFile):
    mifInfo = MifInfo()

    mifF = open(mifFile)
    midF = open(midFile)
    while True:
        line = mifF.readline()
        if len(line) == 0:
            break
        line = line.strip()
        tokens = line.split(" ")
        if len(tokens) == 0:
            continue
        type = tokens[0]
        if type == "" or type == "Version" or type == "Charset" or type == "Delimiter" or type == "CoordSys" or type == "Data":
            continue

        if type == "Columns":
            colNum = int(tokens[1])
            for i in xrange(colNum):
                line = mifF.readline()
                line = line.strip()
                tokens = line.split(" ")
                attrName = tokens[0]
                mifInfo.attrNames.append(attrName)
        elif type == "Point":
            point = Point()
            point.coor.lon = float(tokens[1])
            point.coor.lat = float(tokens[2])
            mifF.readline()
            point.attrs = ReadAttrs(midF)
            mifInfo.points.append(point)
        elif type == "Line":
            line = Line()
            line.coor1.lon = float(tokens[1])
            line.coor1.lat = float(tokens[2])
            line.coor2.lon = float(tokens[3])
            line.coor2.lat = float(tokens[4])
            mifF.readline()
            line.attrs = ReadAttrs(midF)
            mifInfo.lines.append(line)
        elif type == "Pline":
            pline = Pline()
            pointNum = int(tokens[1])
            for i in xrange(pointNum):
                line = mifF.readline()
                line = line.strip()
                tokens = line.split(" ")
                coor = Coor()
                coor.lon = float(tokens[0])
                coor.lat = float(tokens[1])
                pline.coors.append(coor)
            mifF.readline()
            pline.attrs = ReadAttrs(midF)
            mifInfo.plines.append(pline)
        else:
            print "Unknown type: %s" % type
            sys.exit()
    mifF.close()
    midF.close()
    return mifInfo

if len(sys.argv) < 2:
    print "Usage:\n\t%s [MIF file] [MID file]" % sys.argv[0]
    sys.exit()
mifFile = sys.argv[1]
midFile = sys.argv[2]

try:
    mifInfo = ReadMif(mifFile, midFile)
    print mifInfo
except Exception, e:
    print e

