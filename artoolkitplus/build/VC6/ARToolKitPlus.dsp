# Microsoft Developer Studio Project File - Name="ARToolKitPlus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ARToolKitPlus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ARToolKitPlus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ARToolKitPlus.mak" CFG="ARToolKitPlus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ARToolKitPlus - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ARToolKitPlus - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ARToolKitPlus - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "LIBRPP_STATIC" /YX /FD /c
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Win32/ARToolKitPlus.lib"

!ELSEIF  "$(CFG)" == "ARToolKitPlus - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "LIBRPP_STATIC" /YX /FD /GZ /c
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Win32/ARToolKitPlusD.lib"

!ENDIF 

# Begin Target

# Name "ARToolKitPlus - Win32 Release"
# Name "ARToolKitPlus - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\core\arBitFieldPattern.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arDetectMarker.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arDetectMarker2.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arGetCode.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arGetMarkerInfo.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arGetTransMat.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arGetTransMat2.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arGetTransMat3.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arLabeling.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arMultiActivate.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arMultiGetTransMat.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arMultiGetTransMatHull.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arMultiReadConfigFile.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\arUtil.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\matrix.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\mPCA.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\paramDecomp.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\paramDistortion.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\paramFile.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\core\vector.cxx
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Extra"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\extra\BCH.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\extra\FixedPoint.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\extra\FixedPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\extra\Hull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\extra\Hull.h
# End Source File
# Begin Source File

SOURCE=..\..\src\extra\Profiler.cpp
# End Source File
# End Group
# Begin Group "RPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\librpp\librpp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\librpp.h
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp.h
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_const.h
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_quintic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_svd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_types.h
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_vecmat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\librpp\rpp_vecmat.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\CameraAdvImpl.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\CameraFactory.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\CameraImpl.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\MemoryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TrackerImpl.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\TrackerMultiMarkerImpl.cxx
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\src\TrackerSingleMarkerImpl.cxx
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Core  "

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\ar.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\arBitFieldPattern.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\arMulti.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\param.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\vector.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\ARToolKitPlus.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\CameraAdvImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\CameraFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\CameraImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\config.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\ImageGrabber.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\Logger.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\MemoryManager.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\extra\Profiler.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\Tracker.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\TrackerImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\TrackerMultiMarker.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\TrackerMultiMarkerImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\TrackerSingleMarker.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ARToolKitPlus\TrackerSingleMarkerImpl.h
# End Source File
# End Group
# End Target
# End Project
