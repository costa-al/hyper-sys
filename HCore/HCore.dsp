# Microsoft Developer Studio Project File - Name="HCore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=HCore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HCore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HCore.mak" CFG="HCore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HCore - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "HCore - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HCore - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "ASM" /YX /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"../System/HCore.exe"

!ELSEIF  "$(CFG)" == "HCore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ASM" /D "HCORE_API" /Fp"Debug/HPack.pch" /YX /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"../System/HCore.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HCore - Win32 Release"
# Name "HCore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "HPlatform"

# PROP Default_Filter ""
# Begin Group "HWin32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HPlatform\HWin32\HWin32.h
# End Source File
# End Group
# Begin Group "HGnugG"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "HInterfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HInterfaces\HIMaster.h
# End Source File
# End Group
# Begin Group "HMath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HMath\HMath.h
# End Source File
# Begin Source File

SOURCE=.\HMath\HMatrix.h
# End Source File
# Begin Source File

SOURCE=.\HMath\HQuat.h
# End Source File
# Begin Source File

SOURCE=.\HMath\HVec2.h
# End Source File
# Begin Source File

SOURCE=.\HMath\HVec3.h
# End Source File
# End Group
# Begin Group "HScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HScript\HParser.h
# End Source File
# Begin Source File

SOURCE=.\HScript\HScript.h
# End Source File
# Begin Source File

SOURCE=.\HScript\HThread.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\HANSIMalloc.h
# End Source File
# Begin Source File

SOURCE=.\HBase.h
# End Source File
# Begin Source File

SOURCE=.\HBuffer.h
# End Source File
# Begin Source File

SOURCE=.\HCore.h
# End Source File
# Begin Source File

SOURCE=.\HDebugMalloc.h
# End Source File
# Begin Source File

SOURCE=.\HDecl.h
# End Source File
# Begin Source File

SOURCE=.\HEvent.h
# End Source File
# Begin Source File

SOURCE=.\HFile.h
# End Source File
# Begin Source File

SOURCE=.\HObjDecl.h
# End Source File
# Begin Source File

SOURCE=.\HObject.h
# End Source File
# Begin Source File

SOURCE=.\HOutputDeviceNull.h
# End Source File
# Begin Source File

SOURCE=.\HOutputDeviceStdout.h
# End Source File
# Begin Source File

SOURCE=.\HTemplates.h
# End Source File
# Begin Source File

SOURCE=.\HVar.h
# End Source File
# Begin Source File

SOURCE=.\HWindowsPackage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
