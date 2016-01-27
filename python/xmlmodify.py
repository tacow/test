#!/usr/bin/python

from xml.etree import ElementTree

tree = ElementTree.parse('country_data.xml')
root = tree.getroot()
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
