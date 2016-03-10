#!/usr/bin/python
import urlparse

url = "http://www.example.com/path/query?a=1&b=2&b=3&c="
res = urlparse.urlparse(url)
print res.scheme
print res.netloc
print res.path
params = urlparse.parse_qs(res.query, True)
print params
