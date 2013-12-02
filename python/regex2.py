#!/usr/bin/python

import re

param1Pat = "param1=(abc|def|ghi)(&|$)"
param2Pat = "param2=(\d+)(&|$)"

lines = []
lines.append("http://www.example.com/path?param1=abc&param2=111")
lines.append("http://www.example.com/path?param1=def&param2=222")
lines.append("http://www.example.com/path?param1=ghi&param2=333")
lines.append("http://www.example.com/path?param1=jkl&param2=444")
lines.append("http://www.example.com/path?param1=mno&param2=555")
lines.append("http://www.example.com/path?param1=pqr&param2=666")
lines.append("http://www.example.com/path?param1=abc&param2=777&param3=x")
lines.append("http://www.example.com/path?param1=def&param2=888&param3=y")
lines.append("http://www.example.com/path?param1=ghi&param2=999&param3=z")

for line in lines:
    res = re.search(param1Pat, line)
    if res:
        param1 = res.group(1)
        res = re.search(param2Pat, line)
        if res:
            param2 = int(res.group(1))
            print "%s %d" % (param1, param2)

