#!/usr/bin/python
#coding:utf-8

import pynotify
import time,sys

def showMsg(msg):
    pynotify.init ("YGJ@ubuntu")
    str = "[" + time.strftime("%Y-%m-%d %H:%M:%S") + "] " + msg
    bubble_notify = pynotify.Notification ("Attendtion:", str)
    bubble_notify.show ()

if __name__ == '__main__':
    paramsNum = len(sys.argv)
    if paramsNum != 2:
        print 'usage:'
        print 'showMsg.py Msg'
        sys.exit(0)
    showMsg(sys.argv[1])
