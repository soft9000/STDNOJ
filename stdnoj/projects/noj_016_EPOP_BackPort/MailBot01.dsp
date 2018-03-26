# Microsoft Developer Studio Project File - Name="MailBot01" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MailBot01 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MailBot01.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MailBot01.mak" CFG="MailBot01 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MailBot01 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MailBot01 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MailBot01 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "d:\bin"
# PROP Intermediate_Dir "RELEASE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"d:\bin\EPOP.EXE"

!ELSEIF  "$(CFG)" == "MailBot01 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "d:\bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"EPOP.EXE" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MailBot01 - Win32 Release"
# Name "MailBot01 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\server\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\ByteBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\CommonObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\Directory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\Email.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\EMailFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\EmailFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\EMailHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\EMailLines.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\File.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\FilePath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\FileSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdnet\Http.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\HttpLogParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\HyperText01.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\InetRfc.cpp
# End Source File
# Begin Source File

SOURCE=.\MailBot01.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\Node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\NodeFinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\OutboundEmail.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\Platform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\PortTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\ProgramId.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\RfcObjects.cpp
# End Source File
# Begin Source File

SOURCE=..\..\server\SearchData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdnet\Socket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdnet\SocketMonitor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\stdnet\SocketSelect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\StdDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\stdnoj_global.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\StdPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\sockets\StdSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\StdString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\Url.cpp
# End Source File
# Begin Source File

SOURCE=..\..\core\Utilities.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\..\..\lib\WSock32.Lib
# End Source File
# End Target
# End Project
