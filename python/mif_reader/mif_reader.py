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
        self.style = ""
        self.attrs = {}
    def __str__(self):
        attrsStr = ", ".join("%s: %s" % (keyval[0], keyval[1]) for keyval in self.attrs.items())
        return "[Point %s | attributes: %s]" % (self.coor, attrsStr)
    def __repr__(self):
        return str(self)

class Line:
    def __init__(self):
        self.coor1 = Coor()
        self.coor2 = Coor()
        self.style = ""
        self.attrs = {}
    def __str__(self):
        attrsStr = ", ".join("%s: %s" % (keyval[0], keyval[1]) for keyval in self.attrs.items())
        return "[Line %s - %s | attributes: %s]" % (self.coor1, self.coor2, attrsStr)
    def __repr__(self):
        return str(self)

class Pline:
    def __init__(self):
        self.coors = []
        self.style = ""
        self.attrs = {}
    def __str__(self):
        coorsStr = ", ".join(str(coor) for coor in self.coors)
        attrsStr = ", ".join("%s: %s" % (keyval[0], keyval[1]) for keyval in self.attrs.items())
        return "[Pline %s | attributes: %s]" % (coorsStr, attrsStr)
    def __repr__(self):
        return str(self)

class MifInfo:
    def __init__(self):
        self.attrNames = []
        self.attrTypes = []

        self.points = []
        self.lines = []
        self.plines = []
    def __str__(self):
        sections = []
        if self.points:
            pointsStr = "Points\n"
            pointsStr += "-" * 50
            pointsStr += "\n"
            pointsStr += "\n".join(str(point) for point in self.points)
            sections.append(pointsStr)
        if self.lines:
            linesStr = "Lines\n"
            linesStr += "-" * 50
            linesStr += "\n"
            linesStr += "\n".join(str(line) for line in self.lines)
            sections.append(linesStr)
        if self.plines:
            plinesStr = "Plines\n"
            plinesStr += "-" * 50
            plinesStr += "\n"
            plinesStr += "\n".join(str(pline) for pline in self.plines)
            sections.append(plinesStr)
        return "\n\n".join(sections)
    def __repr__(self):
        return str(self)

def ReadStyle(mifF):
    line = mifF.readline()
    return line.strip()

def ReadAttrs(midF, attrNames):
    attrs = {}
    line = midF.readline()
    line = line.strip()
    tokens = line.split(",")
    for i in xrange(len(tokens)):
        attrName = attrNames[i]
        token = tokens[i]
        res = re.match("\"(.*)\"", token)
        if res:
            attr = res.group(1)
        else:
            attr = token
        attrs[attrName] = attr
    return attrs

def WriteAttrs(midF, attrs, attrNames):
    attrsStr = ""
    for attrName in attrNames:
        if attrsStr:
            attrsStr += ","
        if attrs.has_key(attrName):
            attrsStr += "\"%s\"" % attrs[attrName]
        else:
            attrsStr += "\"\""
    midF.write("%s\n" % attrsStr)

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
                tokens = line.split(" ", 1)
                if len(tokens) < 2:
                    continue
                attrName = tokens[0]
                attrType = tokens[1]
                mifInfo.attrNames.append(attrName)
                mifInfo.attrTypes.append(attrType)
        elif type == "Point":
            point = Point()
            point.coor.lon = float(tokens[1])
            point.coor.lat = float(tokens[2])
            point.style = ReadStyle(mifF)
            point.attrs = ReadAttrs(midF, mifInfo.attrNames)
            mifInfo.points.append(point)
        elif type == "Line":
            line = Line()
            line.coor1.lon = float(tokens[1])
            line.coor1.lat = float(tokens[2])
            line.coor2.lon = float(tokens[3])
            line.coor2.lat = float(tokens[4])
            line.style = ReadStyle(mifF)
            line.attrs = ReadAttrs(midF, mifInfo.attrNames)
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
            pline.style = ReadStyle(mifF)
            pline.attrs = ReadAttrs(midF, mifInfo.attrNames)
            mifInfo.plines.append(pline)
        else:
            print "Unknown type: %s" % type
            sys.exit()
    mifF.close()
    midF.close()
    return mifInfo

def WriteMif(mifInfo, mifFile, midFile):
    mifF = open(mifFile, "w")
    midF = open(midFile, "w")
    mifF.write("Version 300\n")
    mifF.write("Charset \"WindowsSimpChinese\"\n")
    mifF.write("Delimiter \",\"\n")
    mifF.write("CoordSys Earth Projection 1, 0\n")
    mifF.write("Columns %d\n" % len(mifInfo.attrNames))
    attrNum = len(mifInfo.attrNames)
    for i in xrange(attrNum):
        attrName = mifInfo.attrNames[i]
        attrType = mifInfo.attrTypes[i]
        mifF.write("  %s %s\n" % (attrName, attrType))
    mifF.write("Data\n\n")

    for point in mifInfo.points:
        mifF.write("Point %f %f\n" % (point.coor.lon, point.coor.lat))
        mifF.write("    %s\n" % point.style)
        WriteAttrs(midF, point.attrs, mifInfo.attrNames)

    for line in mifInfo.lines:
        mifF.write("Line %f %f %f %f\n" % (line.coor1.lon, line.coor1.lat, line.coor2.lon, line.coor2.lat))
        mifF.write("    %s\n" % line.style)
        WriteAttrs(midF, line.attrs, mifInfo.attrNames)

    for pline in mifInfo.plines:
        mifF.write("Pline %d\n" % len(pline.coors))
        for coor in pline.coors:
            mifF.write("%f %f\n" % (coor.lon, coor.lat))
        mifF.write("    %s\n" % pline.style)
        WriteAttrs(midF, pline.attrs, mifInfo.attrNames)

    mifF.close()
    midF.close()

if len(sys.argv) < 2:
    print "Usage:\n\t%s [MIF file] [MID file]" % sys.argv[0]
    sys.exit()
mifFile = sys.argv[1]
midFile = sys.argv[2]

try:
    mifInfo = ReadMif(mifFile, midFile)
    WriteMif(mifInfo, "new.MIF", "new.MID")
except Exception, e:
    print e

