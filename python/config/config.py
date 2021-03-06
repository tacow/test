#!/usr/bin/python

def ReadConfig(filename, sep = ":"):
    try:
        keyVals = {}
        f = open(filename, "r")

        for line in f:
            line = line.strip()
            if line.startswith("#"):
                continue
            res = line.split(sep, 1)
            if len(res) != 2:
                continue
            key = res[0].strip()
            val = res[1].strip()
            keyVals[key] = val

        f.close()
        return keyVals
    except Exception, e:
        print "Load config %s: %s" % (filename, str(e))
        return {}

