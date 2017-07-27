#!/usr/bin/python
import sys
import json

if __name__ == '__main__':
    if len(sys.argv) == 1:
        infile = sys.stdin
        outfile = sys.stdout
    elif len(sys.argv) == 2:
        infile = open(sys.argv[1], 'rb')
        outfile = sys.stdout
    elif len(sys.argv) == 3:
        infile = open(sys.argv[1], 'rb')
        outfile = open(sys.argv[2], 'wb')
    else:
        raise SystemExit("{0} [infile [outfile]]".format(sys.argv[0]))
    try:
        old_content = infile.read()

        obj = json.loads(old_content)
        new_content = json.dumps(obj, ensure_ascii = False, indent=4).encode("utf-8")

        outfile.write(new_content)
    except ValueError, e:
        raise SystemExit(e)
