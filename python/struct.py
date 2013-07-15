#!/usr/bin/python

def Fib(n):
    if n <= 2:
        return 1
    else:
        return Fib(n - 1) + Fib(n - 2)

def Sum(n):
    sum = 0
    for i in xrange(n):
        sum += (i + 1)
    return sum

class Calculator:
    def __init__(self, yourName, num):
        self.yourName = yourName
        self.num = num
    def Display(self):
        print "Hello %s, Fib(%d) = %d, Sum(%d) = %d" % (self.yourName, self.num, Fib(self.num), self.num, Sum(self.num))

calc = Calculator("ABC", 5)
calc.Display()

