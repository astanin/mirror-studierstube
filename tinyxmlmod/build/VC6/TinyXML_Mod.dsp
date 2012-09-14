# Microsoft Developer Studio Project File - Name="TinyXML_Mod" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TinyXML_Mod - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TinyXML_Mod.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TinyXML_Mod.mak" CFG="TinyXML_Mod - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TinyXML_Mod - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TinyXML_Mod - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TinyXML_Mod - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TINYXML_MOD_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../src/TinyXML" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TINYXML_MOD_EXPORTS" /D "TINYXML_MOD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:"../../lib/Win32/TinyXML_Mod.pdb" /machine:I386 /out:"../../bin/Win32/TinyXML_Mod.dll" /implib:"../../lib/Win32/TinyXML_Mod.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TinyXML_Mod - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TINYXML_MOD_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../src/TinyXML" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TINYXML_MOD_EXPORTS" /D "TINYXML_MOD_DLL" /FD /GZ  /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"../../lib/Win32/TinyXML_Modd.pdb" /debug /machine:I386 /out:"../../bin/Win32/TinyXML_Modd.dll" /implib:"../../lib/Win32/TinyXML_Modd.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TinyXML_Mod - Win32 Release"
# Name "TinyXML_Mod - Win32 Debug"
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
