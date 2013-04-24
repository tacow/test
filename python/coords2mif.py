#!/usr/bin/python
import sys

if len(sys.argv) < 2:
    print "Usage: %s [filename] OR" % sys.argv[0]
    print "       %s -" % sys.argv[0]
    sys.exit()
filename = sys.argv[1]

coords = []
if filename == "-":
    f = sys.stdin
else:
    f = open(filename)

for line in f:
    nums = line.split(",")
    while len(nums) >= 2:
        lon = float(nums.pop(0))
        lat = float(nums.pop(0))
        coords.append((lon, lat))

if filename != "-":
    f.close()

print "Version 300"
print "Charset \"WindowsSimpChinese\""
print "Delimiter \",\""
print "CoordSys Earth Projection 1, 0"
print "Columns 1"
print "  name Char(10)"
print "  Data"
print ""
for coord in coords:
    print "Point %f %f" % coord
    print "    Symbol (35,0,12)"

