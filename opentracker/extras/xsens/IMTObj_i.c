/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue May 25 14:39:11 2004
 */
/* Compiler settings for D:\Patrick\XIMU\C++ code\MTObj\IMTObj.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IMotionTracker = {0xBB280863,0x7A0D,0x11D6,{0x92,0x6B,0x00,0x80,0x48,0xB3,0x34,0x83}};


const IID IID_IMotionTrackerEvents = {0xBB280866,0x7A0D,0x11D6,{0x92,0x6B,0x00,0x80,0x48,0xB3,0x34,0x83}};


const IID LIBID_MotionTrackerFilter = {0xBB280864,0x7A0D,0x11D6,{0x92,0x6B,0x00,0x80,0x48,0xB3,0x34,0x83}};


const CLSID CLSID_MotionTracker = {0xBB280865,0x7A0D,0x11D6,{0x92,0x6B,0x00,0x80,0x48,0xB3,0x34,0x83}};


#ifdef __cplusplus
}
#endif

