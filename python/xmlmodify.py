#!/usr/bin/python

from xml.etree import ElementTree

f = open('country_data.xml')
xml = f.read()
f.close()

root = ElementTree.fromstring(xml)
for country in root.findall('country'):
    country.set('name', 'China')

    year = country.find('year')
    if year != None:
        year.text = '2015'

    month = ElementTree.SubElement(country, 'month')
    month.text = '01'

    rank = country.find('rank')
    if rank != None:
        country.remove(rank)
print ElementTree.tostring(root, encoding = "UTF-8")
