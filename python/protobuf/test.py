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

def Read():
    f = open("Person.dat", "rb")
    person = Person_pb2.Person()
    person.ParseFromString(f.read())
    f.close()

    print person.name
    print person.age
    print person.gender
    print person.married
    for friend in person.friends:
        print friend

Write()
Read()

