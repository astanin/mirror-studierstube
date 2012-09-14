/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue May 25 14:39:11 2004
 */
/* Compiler settings for D:\Patrick\XIMU\C++ code\MTObj\IMTObj.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IMTObj_h__
#define __IMTObj_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IMotionTracker_FWD_DEFINED__
#define __IMotionTracker_FWD_DEFINED__
typedef interface IMotionTracker IMotionTracker;
#endif 	/* __IMotionTracker_FWD_DEFINED__ */


#ifndef __IMotionTrackerEvents_FWD_DEFINED__
#define __IMotionTrackerEvents_FWD_DEFINED__
typedef interface IMotionTrackerEvents IMotionTrackerEvents;
#endif 	/* __IMotionTrackerEvents_FWD_DEFINED__ */


#ifndef __MotionTracker_FWD_DEFINED__
#define __MotionTracker_FWD_DEFINED__

#ifdef __cplusplus
typedef class MotionTracker MotionTracker;
#else
typedef struct MotionTracker MotionTracker;
#endif /* __cplusplus */

#endif 	/* __MotionTracker_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IMotionTracker_INTERFACE_DEFINED__
#define __IMotionTracker_INTERFACE_DEFINED__

/* interface IMotionTracker */
/* [oleautomation][dual][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMotionTracker;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB280863-7A0D-11D6-926B-008048B33483")
    IMotionTracker : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MT_SetOutputMode( 
            short nMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetSampleFrequency( 
            short nSampleFreq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetCalibratedOutput( 
            short nEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetCOMPort( 
            short nPort,
            /* [defaultvalue] */ int nBaudrate = 115200) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetxmuLocationBSTR( 
            BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetFilterSettings( 
            float fGain,
            short nCorInterval,
            float fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetDoAMD( 
            short nDoAMD) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetOrientationData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetCalibratedData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetFilterSettings( 
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetNumOrientations( 
            /* [out] */ long __RPC_FAR *nNumOrientations) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_StartProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_StopProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_ResetOrientation( 
            short nResetType,
            /* [defaultvalue] */ short bSaveAfterStop = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetInputPacket( 
            BSTR bstrInputPacket) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetInputPacketB( 
            BSTR bstrInputPacket) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetMTSData( 
            /* [out] */ BSTR __RPC_FAR *bstrMTSData) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetMTSData( 
            BSTR bstrMTSData0,
            /* [defaultvalue] */ BSTR bstrMTSData1 = 0,
            /* [defaultvalue] */ BSTR bstrMTSData2 = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetTimeout( 
            short nTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetxmuLocation( 
            BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_QueryMotionTrackerB( 
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetTimeStampOutput( 
            short nEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetMotionTrackerLocation( 
            short nLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetMotionTrackerLocation( 
            /* [out] */ short __RPC_FAR *nLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetMotionTrackerSampleFrequency( 
            int nSampleFrequency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetMotionTrackerSampleFrequency( 
            /* [out] */ int __RPC_FAR *nSampleFrequency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetMotionTrackerHeading( 
            float fHeading) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetMotionTrackerHeading( 
            /* [out] */ float __RPC_FAR *fHeading) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SetMotionTrackerBaudrate( 
            int nBaudrate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_GetMotionTrackerBaudrate( 
            /* [out] */ int __RPC_FAR *nBaudrate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE MT_SaveToMTS( 
            BSTR bstrDeviceID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_QueryXbusMaster( 
            /* [out] */ short __RPC_FAR *nNumSensors,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID1,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID2,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID3,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID4,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID5) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_QueryXbusMasterB( 
            /* [out] */ short __RPC_FAR *nNumSensors,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceIDs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetOutputMode( 
            short nMode) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetSampleFrequency( 
            short nSampleFreq) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetCalibratedOutput( 
            short nEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetCOMPort( 
            short nPort,
            /* [defaultvalue] */ int nBaudrate = 115200) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetxmuLocationBSTR( 
            BSTR bstrDeviceID,
            BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetFilterSettings( 
            BSTR bstrDeviceID,
            float fGain,
            short nCorInterval,
            float fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetDoAMD( 
            BSTR bstrDeviceID,
            short nDoAMD) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetTimeStampOutput( 
            short nEnabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetOrientationData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetCalibratedData( 
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest = 1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetFilterSettings( 
            BSTR bstrDeviceID,
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetNumOrientations( 
            /* [out] */ long __RPC_FAR *nNumOrientations) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_StartProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_StopProcess( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_ResetOrientation( 
            short nResetType,
            /* [defaultvalue] */ short bSaveAfterStop = 0) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetTimeout( 
            short nTimeOut) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetxmuLocation( 
            BSTR bstrDeviceID,
            BSTR bstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetMotionTrackerLocation( 
            BSTR bstrDeviceID,
            short nLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetMotionTrackerLocation( 
            BSTR bstrDeviceID,
            /* [out] */ short __RPC_FAR *nLocation) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetMotionTrackerHeading( 
            BSTR bstrDeviceID,
            float fHeading) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetMotionTrackerHeading( 
            BSTR bstrDeviceID,
            /* [out] */ float __RPC_FAR *fHeading) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SaveToMTS( 
            BSTR bstrDeviceID) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetXbusMasterSampleFrequency( 
            int nSampleFrequency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_SetXbusMasterBaudrate( 
            int nBaudrate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetXbusMasterSampleFrequency( 
            /* [out] */ int __RPC_FAR *nSampleFrequency) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE XM_GetXbusMasterBaudrate( 
            /* [out] */ int __RPC_FAR *nBaudrate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVersionNumber( 
            /* [out] */ float __RPC_FAR *fVersionNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMotionTrackerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMotionTracker __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMotionTracker __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetOutputMode )( 
            IMotionTracker __RPC_FAR * This,
            short nMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            short nSampleFreq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetCalibratedOutput )( 
            IMotionTracker __RPC_FAR * This,
            short nEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetCOMPort )( 
            IMotionTracker __RPC_FAR * This,
            short nPort,
            /* [defaultvalue] */ int nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetxmuLocationBSTR )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            float fGain,
            short nCorInterval,
            float fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetDoAMD )( 
            IMotionTracker __RPC_FAR * This,
            short nDoAMD);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetOrientationData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetCalibratedData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetNumOrientations )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *nNumOrientations);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_StartProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_StopProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_ResetOrientation )( 
            IMotionTracker __RPC_FAR * This,
            short nResetType,
            /* [defaultvalue] */ short bSaveAfterStop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetInputPacket )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrInputPacket);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetInputPacketB )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrInputPacket);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetMTSData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *bstrMTSData);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetMTSData )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrMTSData0,
            /* [defaultvalue] */ BSTR bstrMTSData1,
            /* [defaultvalue] */ BSTR bstrMTSData2);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetTimeout )( 
            IMotionTracker __RPC_FAR * This,
            short nTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetxmuLocation )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_QueryMotionTrackerB )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetTimeStampOutput )( 
            IMotionTracker __RPC_FAR * This,
            short nEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetMotionTrackerLocation )( 
            IMotionTracker __RPC_FAR * This,
            short nLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetMotionTrackerLocation )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetMotionTrackerSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            int nSampleFrequency);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetMotionTrackerSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *nSampleFrequency);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetMotionTrackerHeading )( 
            IMotionTracker __RPC_FAR * This,
            float fHeading);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetMotionTrackerHeading )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *fHeading);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SetMotionTrackerBaudrate )( 
            IMotionTracker __RPC_FAR * This,
            int nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_GetMotionTrackerBaudrate )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_SaveToMTS )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_QueryXbusMaster )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nNumSensors,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID1,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID2,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID3,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID4,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceID5);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_QueryXbusMasterB )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nNumSensors,
            /* [out] */ BSTR __RPC_FAR *bstrDeviceIDs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetOutputMode )( 
            IMotionTracker __RPC_FAR * This,
            short nMode);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            short nSampleFreq);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetCalibratedOutput )( 
            IMotionTracker __RPC_FAR * This,
            short nEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetCOMPort )( 
            IMotionTracker __RPC_FAR * This,
            short nPort,
            /* [defaultvalue] */ int nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetxmuLocationBSTR )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            float fGain,
            short nCorInterval,
            float fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetDoAMD )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            short nDoAMD);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetTimeStampOutput )( 
            IMotionTracker __RPC_FAR * This,
            short nEnabled);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetOrientationData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetCalibratedData )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ short __RPC_FAR *nRetVal,
            /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
            /* [defaultvalue] */ short nLatest);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetFilterSettings )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            /* [out] */ float __RPC_FAR *fGain,
            /* [out] */ short __RPC_FAR *nCorInterval,
            /* [out] */ float __RPC_FAR *fRho);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetNumOrientations )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *nNumOrientations);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_StartProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_StopProcess )( 
            IMotionTracker __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_ResetOrientation )( 
            IMotionTracker __RPC_FAR * This,
            short nResetType,
            /* [defaultvalue] */ short bSaveAfterStop);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetTimeout )( 
            IMotionTracker __RPC_FAR * This,
            short nTimeOut);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetxmuLocation )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            BSTR bstrName);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetMotionTrackerLocation )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            short nLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetMotionTrackerLocation )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            /* [out] */ short __RPC_FAR *nLocation);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetMotionTrackerHeading )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            float fHeading);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetMotionTrackerHeading )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID,
            /* [out] */ float __RPC_FAR *fHeading);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SaveToMTS )( 
            IMotionTracker __RPC_FAR * This,
            BSTR bstrDeviceID);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetXbusMasterSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            int nSampleFrequency);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_SetXbusMasterBaudrate )( 
            IMotionTracker __RPC_FAR * This,
            int nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetXbusMasterSampleFrequency )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *nSampleFrequency);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *XM_GetXbusMasterBaudrate )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ int __RPC_FAR *nBaudrate);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetVersionNumber )( 
            IMotionTracker __RPC_FAR * This,
            /* [out] */ float __RPC_FAR *fVersionNumber);
        
        END_INTERFACE
    } IMotionTrackerVtbl;

    interface IMotionTracker
    {
        CONST_VTBL struct IMotionTrackerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMotionTracker_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMotionTracker_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMotionTracker_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMotionTracker_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMotionTracker_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMotionTracker_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMotionTracker_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMotionTracker_MT_SetOutputMode(This,nMode)	\
    (This)->lpVtbl -> MT_SetOutputMode(This,nMode)

#define IMotionTracker_MT_SetSampleFrequency(This,nSampleFreq)	\
    (This)->lpVtbl -> MT_SetSampleFrequency(This,nSampleFreq)

#define IMotionTracker_MT_SetCalibratedOutput(This,nEnabled)	\
    (This)->lpVtbl -> MT_SetCalibratedOutput(This,nEnabled)

#define IMotionTracker_MT_SetCOMPort(This,nPort,nBaudrate)	\
    (This)->lpVtbl -> MT_SetCOMPort(This,nPort,nBaudrate)

#define IMotionTracker_MT_SetxmuLocationBSTR(This,bstrName)	\
    (This)->lpVtbl -> MT_SetxmuLocationBSTR(This,bstrName)

#define IMotionTracker_MT_SetFilterSettings(This,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> MT_SetFilterSettings(This,fGain,nCorInterval,fRho)

#define IMotionTracker_MT_SetDoAMD(This,nDoAMD)	\
    (This)->lpVtbl -> MT_SetDoAMD(This,nDoAMD)

#define IMotionTracker_MT_GetOrientationData(This,nRetVal,pfOutputArray,nLatest)	\
    (This)->lpVtbl -> MT_GetOrientationData(This,nRetVal,pfOutputArray,nLatest)

#define IMotionTracker_MT_GetCalibratedData(This,nRetVal,pfOutputArray,nLatest)	\
    (This)->lpVtbl -> MT_GetCalibratedData(This,nRetVal,pfOutputArray,nLatest)

#define IMotionTracker_MT_GetFilterSettings(This,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> MT_GetFilterSettings(This,fGain,nCorInterval,fRho)

#define IMotionTracker_MT_GetNumOrientations(This,nNumOrientations)	\
    (This)->lpVtbl -> MT_GetNumOrientations(This,nNumOrientations)

#define IMotionTracker_MT_StartProcess(This)	\
    (This)->lpVtbl -> MT_StartProcess(This)

#define IMotionTracker_MT_StopProcess(This)	\
    (This)->lpVtbl -> MT_StopProcess(This)

#define IMotionTracker_MT_ResetOrientation(This,nResetType,bSaveAfterStop)	\
    (This)->lpVtbl -> MT_ResetOrientation(This,nResetType,bSaveAfterStop)

#define IMotionTracker_MT_SetInputPacket(This,bstrInputPacket)	\
    (This)->lpVtbl -> MT_SetInputPacket(This,bstrInputPacket)

#define IMotionTracker_MT_SetInputPacketB(This,bstrInputPacket)	\
    (This)->lpVtbl -> MT_SetInputPacketB(This,bstrInputPacket)

#define IMotionTracker_MT_GetMTSData(This,bstrMTSData)	\
    (This)->lpVtbl -> MT_GetMTSData(This,bstrMTSData)

#define IMotionTracker_MT_SetMTSData(This,bstrMTSData0,bstrMTSData1,bstrMTSData2)	\
    (This)->lpVtbl -> MT_SetMTSData(This,bstrMTSData0,bstrMTSData1,bstrMTSData2)

#define IMotionTracker_MT_SetTimeout(This,nTimeOut)	\
    (This)->lpVtbl -> MT_SetTimeout(This,nTimeOut)

#define IMotionTracker_MT_SetxmuLocation(This,bstrName)	\
    (This)->lpVtbl -> MT_SetxmuLocation(This,bstrName)

#define IMotionTracker_MT_QueryMotionTrackerB(This,bstrDeviceID)	\
    (This)->lpVtbl -> MT_QueryMotionTrackerB(This,bstrDeviceID)

#define IMotionTracker_MT_SetTimeStampOutput(This,nEnabled)	\
    (This)->lpVtbl -> MT_SetTimeStampOutput(This,nEnabled)

#define IMotionTracker_MT_SetMotionTrackerLocation(This,nLocation)	\
    (This)->lpVtbl -> MT_SetMotionTrackerLocation(This,nLocation)

#define IMotionTracker_MT_GetMotionTrackerLocation(This,nLocation)	\
    (This)->lpVtbl -> MT_GetMotionTrackerLocation(This,nLocation)

#define IMotionTracker_MT_SetMotionTrackerSampleFrequency(This,nSampleFrequency)	\
    (This)->lpVtbl -> MT_SetMotionTrackerSampleFrequency(This,nSampleFrequency)

#define IMotionTracker_MT_GetMotionTrackerSampleFrequency(This,nSampleFrequency)	\
    (This)->lpVtbl -> MT_GetMotionTrackerSampleFrequency(This,nSampleFrequency)

#define IMotionTracker_MT_SetMotionTrackerHeading(This,fHeading)	\
    (This)->lpVtbl -> MT_SetMotionTrackerHeading(This,fHeading)

#define IMotionTracker_MT_GetMotionTrackerHeading(This,fHeading)	\
    (This)->lpVtbl -> MT_GetMotionTrackerHeading(This,fHeading)

#define IMotionTracker_MT_SetMotionTrackerBaudrate(This,nBaudrate)	\
    (This)->lpVtbl -> MT_SetMotionTrackerBaudrate(This,nBaudrate)

#define IMotionTracker_MT_GetMotionTrackerBaudrate(This,nBaudrate)	\
    (This)->lpVtbl -> MT_GetMotionTrackerBaudrate(This,nBaudrate)

#define IMotionTracker_MT_SaveToMTS(This,bstrDeviceID)	\
    (This)->lpVtbl -> MT_SaveToMTS(This,bstrDeviceID)

#define IMotionTracker_XM_QueryXbusMaster(This,nNumSensors,bstrDeviceID1,bstrDeviceID2,bstrDeviceID3,bstrDeviceID4,bstrDeviceID5)	\
    (This)->lpVtbl -> XM_QueryXbusMaster(This,nNumSensors,bstrDeviceID1,bstrDeviceID2,bstrDeviceID3,bstrDeviceID4,bstrDeviceID5)

#define IMotionTracker_XM_QueryXbusMasterB(This,nNumSensors,bstrDeviceIDs)	\
    (This)->lpVtbl -> XM_QueryXbusMasterB(This,nNumSensors,bstrDeviceIDs)

#define IMotionTracker_XM_SetOutputMode(This,nMode)	\
    (This)->lpVtbl -> XM_SetOutputMode(This,nMode)

#define IMotionTracker_XM_SetSampleFrequency(This,nSampleFreq)	\
    (This)->lpVtbl -> XM_SetSampleFrequency(This,nSampleFreq)

#define IMotionTracker_XM_SetCalibratedOutput(This,nEnabled)	\
    (This)->lpVtbl -> XM_SetCalibratedOutput(This,nEnabled)

#define IMotionTracker_XM_SetCOMPort(This,nPort,nBaudrate)	\
    (This)->lpVtbl -> XM_SetCOMPort(This,nPort,nBaudrate)

#define IMotionTracker_XM_SetxmuLocationBSTR(This,bstrDeviceID,bstrName)	\
    (This)->lpVtbl -> XM_SetxmuLocationBSTR(This,bstrDeviceID,bstrName)

#define IMotionTracker_XM_SetFilterSettings(This,bstrDeviceID,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> XM_SetFilterSettings(This,bstrDeviceID,fGain,nCorInterval,fRho)

#define IMotionTracker_XM_SetDoAMD(This,bstrDeviceID,nDoAMD)	\
    (This)->lpVtbl -> XM_SetDoAMD(This,bstrDeviceID,nDoAMD)

#define IMotionTracker_XM_SetTimeStampOutput(This,nEnabled)	\
    (This)->lpVtbl -> XM_SetTimeStampOutput(This,nEnabled)

#define IMotionTracker_XM_GetOrientationData(This,nRetVal,pfOutputArray,nLatest)	\
    (This)->lpVtbl -> XM_GetOrientationData(This,nRetVal,pfOutputArray,nLatest)

#define IMotionTracker_XM_GetCalibratedData(This,nRetVal,pfOutputArray,nLatest)	\
    (This)->lpVtbl -> XM_GetCalibratedData(This,nRetVal,pfOutputArray,nLatest)

#define IMotionTracker_XM_GetFilterSettings(This,bstrDeviceID,fGain,nCorInterval,fRho)	\
    (This)->lpVtbl -> XM_GetFilterSettings(This,bstrDeviceID,fGain,nCorInterval,fRho)

#define IMotionTracker_XM_GetNumOrientations(This,nNumOrientations)	\
    (This)->lpVtbl -> XM_GetNumOrientations(This,nNumOrientations)

#define IMotionTracker_XM_StartProcess(This)	\
    (This)->lpVtbl -> XM_StartProcess(This)

#define IMotionTracker_XM_StopProcess(This)	\
    (This)->lpVtbl -> XM_StopProcess(This)

#define IMotionTracker_XM_ResetOrientation(This,nResetType,bSaveAfterStop)	\
    (This)->lpVtbl -> XM_ResetOrientation(This,nResetType,bSaveAfterStop)

#define IMotionTracker_XM_SetTimeout(This,nTimeOut)	\
    (This)->lpVtbl -> XM_SetTimeout(This,nTimeOut)

#define IMotionTracker_XM_SetxmuLocation(This,bstrDeviceID,bstrName)	\
    (This)->lpVtbl -> XM_SetxmuLocation(This,bstrDeviceID,bstrName)

#define IMotionTracker_XM_SetMotionTrackerLocation(This,bstrDeviceID,nLocation)	\
    (This)->lpVtbl -> XM_SetMotionTrackerLocation(This,bstrDeviceID,nLocation)

#define IMotionTracker_XM_GetMotionTrackerLocation(This,bstrDeviceID,nLocation)	\
    (This)->lpVtbl -> XM_GetMotionTrackerLocation(This,bstrDeviceID,nLocation)

#define IMotionTracker_XM_SetMotionTrackerHeading(This,bstrDeviceID,fHeading)	\
    (This)->lpVtbl -> XM_SetMotionTrackerHeading(This,bstrDeviceID,fHeading)

#define IMotionTracker_XM_GetMotionTrackerHeading(This,bstrDeviceID,fHeading)	\
    (This)->lpVtbl -> XM_GetMotionTrackerHeading(This,bstrDeviceID,fHeading)

#define IMotionTracker_XM_SaveToMTS(This,bstrDeviceID)	\
    (This)->lpVtbl -> XM_SaveToMTS(This,bstrDeviceID)

#define IMotionTracker_XM_SetXbusMasterSampleFrequency(This,nSampleFrequency)	\
    (This)->lpVtbl -> XM_SetXbusMasterSampleFrequency(This,nSampleFrequency)

#define IMotionTracker_XM_SetXbusMasterBaudrate(This,nBaudrate)	\
    (This)->lpVtbl -> XM_SetXbusMasterBaudrate(This,nBaudrate)

#define IMotionTracker_XM_GetXbusMasterSampleFrequency(This,nSampleFrequency)	\
    (This)->lpVtbl -> XM_GetXbusMasterSampleFrequency(This,nSampleFrequency)

#define IMotionTracker_XM_GetXbusMasterBaudrate(This,nBaudrate)	\
    (This)->lpVtbl -> XM_GetXbusMasterBaudrate(This,nBaudrate)

#define IMotionTracker_GetVersionNumber(This,fVersionNumber)	\
    (This)->lpVtbl -> GetVersionNumber(This,fVersionNumber)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetOutputMode_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nMode);


void __RPC_STUB IMotionTracker_MT_SetOutputMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nSampleFreq);


void __RPC_STUB IMotionTracker_MT_SetSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetCalibratedOutput_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nEnabled);


void __RPC_STUB IMotionTracker_MT_SetCalibratedOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetCOMPort_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nPort,
    /* [defaultvalue] */ int nBaudrate);


void __RPC_STUB IMotionTracker_MT_SetCOMPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetxmuLocationBSTR_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrName);


void __RPC_STUB IMotionTracker_MT_SetxmuLocationBSTR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    float fGain,
    short nCorInterval,
    float fRho);


void __RPC_STUB IMotionTracker_MT_SetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetDoAMD_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nDoAMD);


void __RPC_STUB IMotionTracker_MT_SetDoAMD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetOrientationData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
    /* [defaultvalue] */ short nLatest);


void __RPC_STUB IMotionTracker_MT_GetOrientationData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetCalibratedData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
    /* [defaultvalue] */ short nLatest);


void __RPC_STUB IMotionTracker_MT_GetCalibratedData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *fGain,
    /* [out] */ short __RPC_FAR *nCorInterval,
    /* [out] */ float __RPC_FAR *fRho);


void __RPC_STUB IMotionTracker_MT_GetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetNumOrientations_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *nNumOrientations);


void __RPC_STUB IMotionTracker_MT_GetNumOrientations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_StartProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_MT_StartProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_StopProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_MT_StopProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_ResetOrientation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nResetType,
    /* [defaultvalue] */ short bSaveAfterStop);


void __RPC_STUB IMotionTracker_MT_ResetOrientation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetInputPacket_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrInputPacket);


void __RPC_STUB IMotionTracker_MT_SetInputPacket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetInputPacketB_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrInputPacket);


void __RPC_STUB IMotionTracker_MT_SetInputPacketB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetMTSData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *bstrMTSData);


void __RPC_STUB IMotionTracker_MT_GetMTSData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetMTSData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrMTSData0,
    /* [defaultvalue] */ BSTR bstrMTSData1,
    /* [defaultvalue] */ BSTR bstrMTSData2);


void __RPC_STUB IMotionTracker_MT_SetMTSData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetTimeout_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nTimeOut);


void __RPC_STUB IMotionTracker_MT_SetTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetxmuLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrName);


void __RPC_STUB IMotionTracker_MT_SetxmuLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_QueryMotionTrackerB_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID);


void __RPC_STUB IMotionTracker_MT_QueryMotionTrackerB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetTimeStampOutput_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nEnabled);


void __RPC_STUB IMotionTracker_MT_SetTimeStampOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetMotionTrackerLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nLocation);


void __RPC_STUB IMotionTracker_MT_SetMotionTrackerLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetMotionTrackerLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nLocation);


void __RPC_STUB IMotionTracker_MT_GetMotionTrackerLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetMotionTrackerSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    int nSampleFrequency);


void __RPC_STUB IMotionTracker_MT_SetMotionTrackerSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetMotionTrackerSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *nSampleFrequency);


void __RPC_STUB IMotionTracker_MT_GetMotionTrackerSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetMotionTrackerHeading_Proxy( 
    IMotionTracker __RPC_FAR * This,
    float fHeading);


void __RPC_STUB IMotionTracker_MT_SetMotionTrackerHeading_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetMotionTrackerHeading_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *fHeading);


void __RPC_STUB IMotionTracker_MT_GetMotionTrackerHeading_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SetMotionTrackerBaudrate_Proxy( 
    IMotionTracker __RPC_FAR * This,
    int nBaudrate);


void __RPC_STUB IMotionTracker_MT_SetMotionTrackerBaudrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_GetMotionTrackerBaudrate_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *nBaudrate);


void __RPC_STUB IMotionTracker_MT_GetMotionTrackerBaudrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_MT_SaveToMTS_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID);


void __RPC_STUB IMotionTracker_MT_SaveToMTS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_QueryXbusMaster_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nNumSensors,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID1,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID2,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID3,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID4,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceID5);


void __RPC_STUB IMotionTracker_XM_QueryXbusMaster_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_QueryXbusMasterB_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nNumSensors,
    /* [out] */ BSTR __RPC_FAR *bstrDeviceIDs);


void __RPC_STUB IMotionTracker_XM_QueryXbusMasterB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetOutputMode_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nMode);


void __RPC_STUB IMotionTracker_XM_SetOutputMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nSampleFreq);


void __RPC_STUB IMotionTracker_XM_SetSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetCalibratedOutput_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nEnabled);


void __RPC_STUB IMotionTracker_XM_SetCalibratedOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetCOMPort_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nPort,
    /* [defaultvalue] */ int nBaudrate);


void __RPC_STUB IMotionTracker_XM_SetCOMPort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetxmuLocationBSTR_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    BSTR bstrName);


void __RPC_STUB IMotionTracker_XM_SetxmuLocationBSTR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    float fGain,
    short nCorInterval,
    float fRho);


void __RPC_STUB IMotionTracker_XM_SetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetDoAMD_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    short nDoAMD);


void __RPC_STUB IMotionTracker_XM_SetDoAMD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetTimeStampOutput_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nEnabled);


void __RPC_STUB IMotionTracker_XM_SetTimeStampOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetOrientationData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
    /* [defaultvalue] */ short nLatest);


void __RPC_STUB IMotionTracker_XM_GetOrientationData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetCalibratedData_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ short __RPC_FAR *nRetVal,
    /* [out] */ VARIANT __RPC_FAR *pfOutputArray,
    /* [defaultvalue] */ short nLatest);


void __RPC_STUB IMotionTracker_XM_GetCalibratedData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetFilterSettings_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    /* [out] */ float __RPC_FAR *fGain,
    /* [out] */ short __RPC_FAR *nCorInterval,
    /* [out] */ float __RPC_FAR *fRho);


void __RPC_STUB IMotionTracker_XM_GetFilterSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetNumOrientations_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *nNumOrientations);


void __RPC_STUB IMotionTracker_XM_GetNumOrientations_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_StartProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_XM_StartProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_StopProcess_Proxy( 
    IMotionTracker __RPC_FAR * This);


void __RPC_STUB IMotionTracker_XM_StopProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_ResetOrientation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nResetType,
    /* [defaultvalue] */ short bSaveAfterStop);


void __RPC_STUB IMotionTracker_XM_ResetOrientation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetTimeout_Proxy( 
    IMotionTracker __RPC_FAR * This,
    short nTimeOut);


void __RPC_STUB IMotionTracker_XM_SetTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetxmuLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    BSTR bstrName);


void __RPC_STUB IMotionTracker_XM_SetxmuLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetMotionTrackerLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    short nLocation);


void __RPC_STUB IMotionTracker_XM_SetMotionTrackerLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetMotionTrackerLocation_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    /* [out] */ short __RPC_FAR *nLocation);


void __RPC_STUB IMotionTracker_XM_GetMotionTrackerLocation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetMotionTrackerHeading_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    float fHeading);


void __RPC_STUB IMotionTracker_XM_SetMotionTrackerHeading_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetMotionTrackerHeading_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID,
    /* [out] */ float __RPC_FAR *fHeading);


void __RPC_STUB IMotionTracker_XM_GetMotionTrackerHeading_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SaveToMTS_Proxy( 
    IMotionTracker __RPC_FAR * This,
    BSTR bstrDeviceID);


void __RPC_STUB IMotionTracker_XM_SaveToMTS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetXbusMasterSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    int nSampleFrequency);


void __RPC_STUB IMotionTracker_XM_SetXbusMasterSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_SetXbusMasterBaudrate_Proxy( 
    IMotionTracker __RPC_FAR * This,
    int nBaudrate);


void __RPC_STUB IMotionTracker_XM_SetXbusMasterBaudrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetXbusMasterSampleFrequency_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *nSampleFrequency);


void __RPC_STUB IMotionTracker_XM_GetXbusMasterSampleFrequency_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_XM_GetXbusMasterBaudrate_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ int __RPC_FAR *nBaudrate);


void __RPC_STUB IMotionTracker_XM_GetXbusMasterBaudrate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMotionTracker_GetVersionNumber_Proxy( 
    IMotionTracker __RPC_FAR * This,
    /* [out] */ float __RPC_FAR *fVersionNumber);


void __RPC_STUB IMotionTracker_GetVersionNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMotionTracker_INTERFACE_DEFINED__ */


#ifndef __IMotionTrackerEvents_INTERFACE_DEFINED__
#define __IMotionTrackerEvents_INTERFACE_DEFINED__

/* interface IMotionTrackerEvents */
/* [oleautomation][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMotionTrackerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BB280866-7A0D-11D6-926B-008048B33483")
    IMotionTrackerEvents : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE MT_OrientationChanged( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMotionTrackerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IMotionTrackerEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *MT_OrientationChanged )( 
            IMotionTrackerEvents __RPC_FAR * This);
        
        END_INTERFACE
    } IMotionTrackerEventsVtbl;

    interface IMotionTrackerEvents
    {
        CONST_VTBL struct IMotionTrackerEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMotionTrackerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMotionTrackerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMotionTrackerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMotionTrackerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMotionTrackerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMotionTrackerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMotionTrackerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMotionTrackerEvents_MT_OrientationChanged(This)	\
    (This)->lpVtbl -> MT_OrientationChanged(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMotionTrackerEvents_MT_OrientationChanged_Proxy( 
    IMotionTrackerEvents __RPC_FAR * This);


void __RPC_STUB IMotionTrackerEvents_MT_OrientationChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMotionTrackerEvents_INTERFACE_DEFINED__ */



#ifndef __MotionTrackerFilter_LIBRARY_DEFINED__
#define __MotionTrackerFilter_LIBRARY_DEFINED__

/* library MotionTrackerFilter */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MotionTrackerFilter;

EXTERN_C const CLSID CLSID_MotionTracker;

#ifdef __cplusplus

class DECLSPEC_UUID("BB280865-7A0D-11D6-926B-008048B33483")
MotionTracker;
#endif
#endif /* __MotionTrackerFilter_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
