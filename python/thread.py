#!/usr/bin/python
import threading
import time

class timer(threading.Thread):
    def __init__(self, num, interval):
        threading.Thread.__init__(self)
        self.thread_num = num
        self.interval = interval
 
    def run(self):
        for i in xrange(5):
            print 'Thread Object(%d), Time: %s' % (self.thread_num, time.ctime())
            time.sleep(self.interval)
 
def test():
    thread1 = timer(1, 1)
    thread2 = timer(2, 2)
    thread1.start()
    thread2.start()
    thread1.join()
    thread2.join()
 
if __name__ == '__main__':
    test()

