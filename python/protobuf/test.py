#! /usr/bin/python

import Person_pb2
import sys

def Write():
    person = Person_pb2.Person()
    person.name = "aaa"
    person.age = 25
    person.gender = Person_pb2.Male
    person.married = True
    person.friends.append("bbb")
    person.friends.append("ccc")

    f = open("Person.dat", "wb")
    f.write(person.SerializeToString())
    f.close()

def Write2():
    person = Person_pb2.Person()
    person.age = 20
    person.gender = Person_pb2.Female
    person.married = False

    f = open("Person.dat", "wb")
    f.write(person.SerializeToString())
    f.close()

def Read():
    f = open("Person.dat", "rb")
    person = Person_pb2.Person()
    person.ParseFromString(f.read())
    f.close()

    if person.HasField("name"):
        print "Name:", person.name
    print "Age:", person.age
    print "Gender:", person.gender
    print "Married:", person.married
    for friend in person.friends:
        print "Friend:", friend

Write()
Read()

print "=============================="

Write2()
Read()

