# Microsoft Developer Studio Project File - Name="axcontrol" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axcontrol - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axcontrol.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axcontrol.mak" CFG="axcontrol - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axcontrol - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axcontrol - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Python/com/win32comext/axcontrol", LFCAAAAA"
# PROP Scc_LocalPath "win32comext/axcontrol"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axcontrol - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "Build\Temp\axcontrol\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\com\win32com\src\include" /I "..\win32\src" /D "WIN32" /D "_WINDOWS" /D "NDEBUG" /D "STRICT" /YX"axcontrol_pch.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /base:"0x1e830000" /subsystem:windows /dll /debug /machine:I386 /out:"Build\axcontrol.pyd" /libpath:"..\win32\build"

!ELSEIF  "$(CFG)" == "axcontrol - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "Build\Temp\axcontrol\debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "..\com\win32com\src\include" /I "..\win32\src" /D "WIN32" /D "_WINDOWS" /D "_DEBUG" /D "STRICT" /YX"axcontrol_pch.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /base:"0x1e830000" /subsystem:windows /dll /debug /machine:I386 /out:"Build\axcontrol_d.pyd" /pdbtype:sept /libpath:"..\win32\build"

!ENDIF 

# Begin Target

# Name "axcontrol - Win32 Release"
# Name "axcontrol - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cxx;cpp;c"
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\AXControl.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleClientSite.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleControl.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleInPlaceObject.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleInPlaceSite.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleInPlaceSiteEx.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleInPlaceSiteWindowless.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleObject.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyISpecifyPropertyPages.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIViewObject.cpp
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIViewObject2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\axcontrol_pch.h
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleClientSite.h
# End Source File
# Begin Source File

SOURCE=.\win32comext\axcontrol\src\PyIOleObject.h
# End Source File
# End Group
# End Target
# End Project
