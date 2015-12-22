#!/usr/bin/python
import sys
from mif_reader import *

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

