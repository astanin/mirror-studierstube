======================================================================
The DLL included in this distribution provides an easy to use API 
that can detect, configure and retrieve data from all models of 
InterSense trackers. A sample application is provided to illustrate 
the use of most functions. See isense.h for explanations and function 
prototypes. Significant portion of the API and the sample application 
is not relevant when used with InterTrax. 


SERIAL PORT MAPPING (UNIX)
======================================================================
Device names for serial ports vary on different versions of Unix. 
The library uses default names for each of the supported operating 
system. That, however, is not always sufficient, particularly when 
USB to Serial converters are used. To solve this, a configuration 
file isports.ini can be used to specify port number to device string 
mapping. The included sample file was produced for Mac OS X and 
contains lines for Keyspan and InterSense USB converters. Please
check your system for specific device names. 

PortX = device:baudRate

X is replaced with the logical port number starting at 1. "device"
can be a path to the device driver, like /dev/ttyS0. Baud rate
parameter is optional. All baud rates are tested to detect the
tracker. By default, once tracker is detected, the baud rate is 
changed to 115200 for best performance. If you are using a very 
long serial cable, you may want to use this feature to force
the lower baud rate. This feature is only available for Windows
version in this release.

Note: If isports.ini file is found, all required ports must be 
defined in it. Only devices listed in the file will be searched.


TCP SUPPORT
======================================================================
Windows and Unix versions support communication to InterSense 
tracking devices over TCP/IP. To use this feature you must
define the connection string in the isports.ini file in following
form: PortX = IP:Port. In the included sample isports.ini file
you will see it as Port3 = 192.168.1.44:5005


INSTALATION
======================================================================

1. Windows9x
   Copy isense.dll to C:\Windows\System

2. Windows NT/2000/XP
   Login as Administrator
   Copy isense.dll to C:\WINNT\SYSTEM32

3. Linux
   Login as root
   Copy Linux/libisense.so to /lib directory

4. IRIX
   Login as root
   Copy Irix/libisense.so to /usr/lib32 directory

5. Mac OS X
   Login as root
   Copy libisense.dylib to /usr/lib directory

6. HP Unix
   Login as root
   Copy libisense.sl to /usr/lib directory
   Make sure that libisense.sl has executable privileges,
   if needed run "chmod +x /usr/lib/libisense.sl"
   Make sure the SHLIB_PATH environmental variable contains /usr/lib


CONTACT INFORMATION
======================================================================

InterSense Inc.
36 Crosby Dr., STE 150
Bedford, MA
01730 USA

Tel. 781-541-6330
Fax. 781-541-6329

www.isense.com