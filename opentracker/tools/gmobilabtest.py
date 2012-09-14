#!/usr/bin/python

import serial
import binascii
import sys
import getopt

def testbci():
    ser = serial.Serial("/dev/ttyUSB0",baudrate=57600,timeout=3)

    if not ser.isOpen():
        print "could not open serial port\n"
        return
        
    # this is the initialization string
    outstr = "c\255 "

    print "Sending device initialization command for all channels ..."

    nwritten = ser.write(outstr)

    #print "Chars to read: " + str(ser.inWaiting())
    rc = ser.read()



    if len(rc) == 1 and rc == 'c':
        print " success!"
    else:
        print " failed! (no reply from device)"
        return
    
    print "Sending device start command ... "
    ser.write('a')

    #print ser.inWaiting()

    maxcount = 128
    curcount = 0

    while curcount < maxcount:
        readdata = ser.read(16)
        if len(readdata) == 0:
            print "received no data from device!"
            curcount = maxcount
        
        printstr = ""
        for c in readdata:
            printstr = printstr + '%02X'%ord(c) + " "
            curcount = curcount + 1
        if len(printstr) > 0:
            print printstr

    print "Sending stop command ..." 
    ser.write('b')

    print "Still waiting: " + str(ser.inWaiting())
    laststring = ser.read(ser.inWaiting())
    print "Still waiting: " + str(ser.inWaiting())

    # eventually flush input buffer
    #ser.flushInput()
    #ser.flushOutput()
    
    #print "Still waiting: " + str(ser.inWaiting())
    ser.close()


if __name__ == '__main__':
    opts,args = getopt.getopt(sys.argv[1:],'','')
    testbci()
