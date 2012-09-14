# Microsoft Developer Studio Project File - Name="TinyXML_ModLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TinyXML_ModLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TinyXML_ModLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TinyXML_ModLib.mak" CFG="TinyXML_ModLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TinyXML_ModLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TinyXML_ModLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TinyXML_ModLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TinyXML_ModLib___Win32_Release"
# PROP BASE Intermediate_Dir "TinyXML_ModLib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseLib"
# PROP Intermediate_Dir "ReleaseLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../src/TinyXML" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "TINYXML_MOD_STATIC" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Win32/TinyXML_ModLib.lib"

!ELSEIF  "$(CFG)" == "TinyXML_ModLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TinyXML_ModLib___Win32_Debug"
# PROP BASE Intermediate_Dir "TinyXML_ModLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugLib"
# PROP Intermediate_Dir "DebugLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../src/TinyXML" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "TINYXML_MOD_STATIC" /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Win32/TinyXML_ModLibd.lib"

!ENDIF 

# Begin Target

# Name "TinyXML_ModLib - Win32 Release"
# Name "TinyXML_ModLib - Win32 Debug"
# Begin Group "TinyXML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\TinyXML\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\tinystr.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\tinyxml.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\TinyXMLMemManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\TinyXMLMemManager.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXML\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "TinyXPath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\TinyXPath\action_store.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\action_store.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\byte_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\lex_token.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\lex_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\lex_util.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\node_set.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\node_set.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\tinyxpath_conf.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\tokenlist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\tokenlist.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xml_util.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xml_util.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_expression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_expression.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_processor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_processor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_stack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_stack.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_static.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_static.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_stream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_stream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_syntax.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TinyXPath\xpath_syntax.h
# End Source File
# End Group
# End Target
# End Project
