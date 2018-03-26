# Microsoft Developer Studio Generated NMAKE File, Based on MailBot01.dsp
!IF "$(CFG)" == ""
CFG=MailBot01 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MailBot01 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MailBot01 - Win32 Release" && "$(CFG)" != "MailBot01 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MailBot01 - Win32 Release"

OUTDIR=d:\bin
INTDIR=.\RELEASE
# Begin Custom Macros
OutDir=d:\bin
# End Custom Macros

ALL : "$(OUTDIR)\EPOP.EXE" "$(OUTDIR)\MailBot01.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\ByteBuffer.obj"
	-@erase "$(INTDIR)\ByteBuffer.sbr"
	-@erase "$(INTDIR)\CommonObject.obj"
	-@erase "$(INTDIR)\CommonObject.sbr"
	-@erase "$(INTDIR)\Directory.obj"
	-@erase "$(INTDIR)\Directory.sbr"
	-@erase "$(INTDIR)\Email.obj"
	-@erase "$(INTDIR)\Email.sbr"
	-@erase "$(INTDIR)\EMailFactory.obj"
	-@erase "$(INTDIR)\EMailFactory.sbr"
	-@erase "$(INTDIR)\EmailFilter.obj"
	-@erase "$(INTDIR)\EmailFilter.sbr"
	-@erase "$(INTDIR)\EMailHeader.obj"
	-@erase "$(INTDIR)\EMailHeader.sbr"
	-@erase "$(INTDIR)\EMailLines.obj"
	-@erase "$(INTDIR)\EMailLines.sbr"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\File.sbr"
	-@erase "$(INTDIR)\FilePath.obj"
	-@erase "$(INTDIR)\FilePath.sbr"
	-@erase "$(INTDIR)\FileSystem.obj"
	-@erase "$(INTDIR)\FileSystem.sbr"
	-@erase "$(INTDIR)\Http.obj"
	-@erase "$(INTDIR)\Http.sbr"
	-@erase "$(INTDIR)\HttpLogParser.obj"
	-@erase "$(INTDIR)\HttpLogParser.sbr"
	-@erase "$(INTDIR)\HyperText01.obj"
	-@erase "$(INTDIR)\HyperText01.sbr"
	-@erase "$(INTDIR)\InetRfc.obj"
	-@erase "$(INTDIR)\InetRfc.sbr"
	-@erase "$(INTDIR)\MailBot01.obj"
	-@erase "$(INTDIR)\MailBot01.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\Node.obj"
	-@erase "$(INTDIR)\Node.sbr"
	-@erase "$(INTDIR)\NodeFinder.obj"
	-@erase "$(INTDIR)\NodeFinder.sbr"
	-@erase "$(INTDIR)\OutboundEmail.obj"
	-@erase "$(INTDIR)\OutboundEmail.sbr"
	-@erase "$(INTDIR)\Platform.obj"
	-@erase "$(INTDIR)\Platform.sbr"
	-@erase "$(INTDIR)\PortTime.obj"
	-@erase "$(INTDIR)\PortTime.sbr"
	-@erase "$(INTDIR)\ProgramId.obj"
	-@erase "$(INTDIR)\ProgramId.sbr"
	-@erase "$(INTDIR)\RfcObjects.obj"
	-@erase "$(INTDIR)\RfcObjects.sbr"
	-@erase "$(INTDIR)\SearchData.obj"
	-@erase "$(INTDIR)\SearchData.sbr"
	-@erase "$(INTDIR)\Socket.obj"
	-@erase "$(INTDIR)\Socket.sbr"
	-@erase "$(INTDIR)\SocketMonitor.obj"
	-@erase "$(INTDIR)\SocketMonitor.sbr"
	-@erase "$(INTDIR)\SocketSelect.obj"
	-@erase "$(INTDIR)\SocketSelect.sbr"
	-@erase "$(INTDIR)\StdDateTime.obj"
	-@erase "$(INTDIR)\StdDateTime.sbr"
	-@erase "$(INTDIR)\stdnoj_global.obj"
	-@erase "$(INTDIR)\stdnoj_global.sbr"
	-@erase "$(INTDIR)\StdPath.obj"
	-@erase "$(INTDIR)\StdPath.sbr"
	-@erase "$(INTDIR)\StdSocket.obj"
	-@erase "$(INTDIR)\StdSocket.sbr"
	-@erase "$(INTDIR)\StdString.obj"
	-@erase "$(INTDIR)\StdString.sbr"
	-@erase "$(INTDIR)\Url.obj"
	-@erase "$(INTDIR)\Url.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\EPOP.EXE"
	-@erase "$(OUTDIR)\MailBot01.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MailBot01.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MailBot01.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MailBot01.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\ByteBuffer.sbr" \
	"$(INTDIR)\CommonObject.sbr" \
	"$(INTDIR)\Directory.sbr" \
	"$(INTDIR)\File.sbr" \
	"$(INTDIR)\FilePath.sbr" \
	"$(INTDIR)\FileSystem.sbr" \
	"$(INTDIR)\Node.sbr" \
	"$(INTDIR)\NodeFinder.sbr" \
	"$(INTDIR)\Platform.sbr" \
	"$(INTDIR)\PortTime.sbr" \
	"$(INTDIR)\ProgramId.sbr" \
	"$(INTDIR)\StdDateTime.sbr" \
	"$(INTDIR)\stdnoj_global.sbr" \
	"$(INTDIR)\StdPath.sbr" \
	"$(INTDIR)\StdString.sbr" \
	"$(INTDIR)\Url.sbr" \
	"$(INTDIR)\SearchData.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\Email.sbr" \
	"$(INTDIR)\EmailFilter.sbr" \
	"$(INTDIR)\EMailHeader.sbr" \
	"$(INTDIR)\EMailLines.sbr" \
	"$(INTDIR)\HttpLogParser.sbr" \
	"$(INTDIR)\HyperText01.sbr" \
	"$(INTDIR)\InetRfc.sbr" \
	"$(INTDIR)\OutboundEmail.sbr" \
	"$(INTDIR)\RfcObjects.sbr" \
	"$(INTDIR)\Http.sbr" \
	"$(INTDIR)\Socket.sbr" \
	"$(INTDIR)\SocketMonitor.sbr" \
	"$(INTDIR)\SocketSelect.sbr" \
	"$(INTDIR)\StdSocket.sbr" \
	"$(INTDIR)\EMailFactory.sbr"

"$(OUTDIR)\MailBot01.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\EPOP.pdb" /machine:I386 /out:"$(OUTDIR)\EPOP.EXE" 
LINK32_OBJS= \
	"$(INTDIR)\MailBot01.obj" \
	"$(INTDIR)\main.obj" \
	"..\..\..\..\lib\WSock32.Lib" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\ByteBuffer.obj" \
	"$(INTDIR)\CommonObject.obj" \
	"$(INTDIR)\Directory.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FilePath.obj" \
	"$(INTDIR)\FileSystem.obj" \
	"$(INTDIR)\Node.obj" \
	"$(INTDIR)\NodeFinder.obj" \
	"$(INTDIR)\Platform.obj" \
	"$(INTDIR)\PortTime.obj" \
	"$(INTDIR)\ProgramId.obj" \
	"$(INTDIR)\StdDateTime.obj" \
	"$(INTDIR)\stdnoj_global.obj" \
	"$(INTDIR)\StdPath.obj" \
	"$(INTDIR)\StdString.obj" \
	"$(INTDIR)\Url.obj" \
	"$(INTDIR)\SearchData.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\Email.obj" \
	"$(INTDIR)\EmailFilter.obj" \
	"$(INTDIR)\EMailHeader.obj" \
	"$(INTDIR)\EMailLines.obj" \
	"$(INTDIR)\HttpLogParser.obj" \
	"$(INTDIR)\HyperText01.obj" \
	"$(INTDIR)\InetRfc.obj" \
	"$(INTDIR)\OutboundEmail.obj" \
	"$(INTDIR)\RfcObjects.obj" \
	"$(INTDIR)\Http.obj" \
	"$(INTDIR)\Socket.obj" \
	"$(INTDIR)\SocketMonitor.obj" \
	"$(INTDIR)\SocketSelect.obj" \
	"$(INTDIR)\StdSocket.obj" \
	"$(INTDIR)\EMailFactory.obj"

"$(OUTDIR)\EPOP.EXE" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MailBot01 - Win32 Debug"

OUTDIR=d:\bin
INTDIR=.\Debug
# Begin Custom Macros
OutDir=d:\bin
# End Custom Macros

ALL : ".\EPOP.EXE" "$(OUTDIR)\MailBot01.bsc"


CLEAN :
	-@erase "$(INTDIR)\Base64.obj"
	-@erase "$(INTDIR)\Base64.sbr"
	-@erase "$(INTDIR)\ByteBuffer.obj"
	-@erase "$(INTDIR)\ByteBuffer.sbr"
	-@erase "$(INTDIR)\CommonObject.obj"
	-@erase "$(INTDIR)\CommonObject.sbr"
	-@erase "$(INTDIR)\Directory.obj"
	-@erase "$(INTDIR)\Directory.sbr"
	-@erase "$(INTDIR)\Email.obj"
	-@erase "$(INTDIR)\Email.sbr"
	-@erase "$(INTDIR)\EMailFactory.obj"
	-@erase "$(INTDIR)\EMailFactory.sbr"
	-@erase "$(INTDIR)\EmailFilter.obj"
	-@erase "$(INTDIR)\EmailFilter.sbr"
	-@erase "$(INTDIR)\EMailHeader.obj"
	-@erase "$(INTDIR)\EMailHeader.sbr"
	-@erase "$(INTDIR)\EMailLines.obj"
	-@erase "$(INTDIR)\EMailLines.sbr"
	-@erase "$(INTDIR)\File.obj"
	-@erase "$(INTDIR)\File.sbr"
	-@erase "$(INTDIR)\FilePath.obj"
	-@erase "$(INTDIR)\FilePath.sbr"
	-@erase "$(INTDIR)\FileSystem.obj"
	-@erase "$(INTDIR)\FileSystem.sbr"
	-@erase "$(INTDIR)\Http.obj"
	-@erase "$(INTDIR)\Http.sbr"
	-@erase "$(INTDIR)\HttpLogParser.obj"
	-@erase "$(INTDIR)\HttpLogParser.sbr"
	-@erase "$(INTDIR)\HyperText01.obj"
	-@erase "$(INTDIR)\HyperText01.sbr"
	-@erase "$(INTDIR)\InetRfc.obj"
	-@erase "$(INTDIR)\InetRfc.sbr"
	-@erase "$(INTDIR)\MailBot01.obj"
	-@erase "$(INTDIR)\MailBot01.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\Node.obj"
	-@erase "$(INTDIR)\Node.sbr"
	-@erase "$(INTDIR)\NodeFinder.obj"
	-@erase "$(INTDIR)\NodeFinder.sbr"
	-@erase "$(INTDIR)\OutboundEmail.obj"
	-@erase "$(INTDIR)\OutboundEmail.sbr"
	-@erase "$(INTDIR)\Platform.obj"
	-@erase "$(INTDIR)\Platform.sbr"
	-@erase "$(INTDIR)\PortTime.obj"
	-@erase "$(INTDIR)\PortTime.sbr"
	-@erase "$(INTDIR)\ProgramId.obj"
	-@erase "$(INTDIR)\ProgramId.sbr"
	-@erase "$(INTDIR)\RfcObjects.obj"
	-@erase "$(INTDIR)\RfcObjects.sbr"
	-@erase "$(INTDIR)\SearchData.obj"
	-@erase "$(INTDIR)\SearchData.sbr"
	-@erase "$(INTDIR)\Socket.obj"
	-@erase "$(INTDIR)\Socket.sbr"
	-@erase "$(INTDIR)\SocketMonitor.obj"
	-@erase "$(INTDIR)\SocketMonitor.sbr"
	-@erase "$(INTDIR)\SocketSelect.obj"
	-@erase "$(INTDIR)\SocketSelect.sbr"
	-@erase "$(INTDIR)\StdDateTime.obj"
	-@erase "$(INTDIR)\StdDateTime.sbr"
	-@erase "$(INTDIR)\stdnoj_global.obj"
	-@erase "$(INTDIR)\stdnoj_global.sbr"
	-@erase "$(INTDIR)\StdPath.obj"
	-@erase "$(INTDIR)\StdPath.sbr"
	-@erase "$(INTDIR)\StdSocket.obj"
	-@erase "$(INTDIR)\StdSocket.sbr"
	-@erase "$(INTDIR)\StdString.obj"
	-@erase "$(INTDIR)\StdString.sbr"
	-@erase "$(INTDIR)\Url.obj"
	-@erase "$(INTDIR)\Url.sbr"
	-@erase "$(INTDIR)\Utilities.obj"
	-@erase "$(INTDIR)\Utilities.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\EPOP.pdb"
	-@erase "$(OUTDIR)\MailBot01.bsc"
	-@erase ".\EPOP.EXE"
	-@erase ".\EPOP.ILK"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\MailBot01.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MailBot01.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\MailBot01.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\Utilities.sbr" \
	"$(INTDIR)\ByteBuffer.sbr" \
	"$(INTDIR)\CommonObject.sbr" \
	"$(INTDIR)\Directory.sbr" \
	"$(INTDIR)\File.sbr" \
	"$(INTDIR)\FilePath.sbr" \
	"$(INTDIR)\FileSystem.sbr" \
	"$(INTDIR)\Node.sbr" \
	"$(INTDIR)\NodeFinder.sbr" \
	"$(INTDIR)\Platform.sbr" \
	"$(INTDIR)\PortTime.sbr" \
	"$(INTDIR)\ProgramId.sbr" \
	"$(INTDIR)\StdDateTime.sbr" \
	"$(INTDIR)\stdnoj_global.sbr" \
	"$(INTDIR)\StdPath.sbr" \
	"$(INTDIR)\StdString.sbr" \
	"$(INTDIR)\Url.sbr" \
	"$(INTDIR)\SearchData.sbr" \
	"$(INTDIR)\Base64.sbr" \
	"$(INTDIR)\Email.sbr" \
	"$(INTDIR)\EmailFilter.sbr" \
	"$(INTDIR)\EMailHeader.sbr" \
	"$(INTDIR)\EMailLines.sbr" \
	"$(INTDIR)\HttpLogParser.sbr" \
	"$(INTDIR)\HyperText01.sbr" \
	"$(INTDIR)\InetRfc.sbr" \
	"$(INTDIR)\OutboundEmail.sbr" \
	"$(INTDIR)\RfcObjects.sbr" \
	"$(INTDIR)\Http.sbr" \
	"$(INTDIR)\Socket.sbr" \
	"$(INTDIR)\SocketMonitor.sbr" \
	"$(INTDIR)\SocketSelect.sbr" \
	"$(INTDIR)\StdSocket.sbr" \
	"$(INTDIR)\EMailFactory.sbr"

"$(OUTDIR)\MailBot01.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\EPOP.pdb" /debug /machine:I386 /out:"EPOP.EXE" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\MailBot01.obj" \
	"$(INTDIR)\main.obj" \
	"..\..\..\..\lib\WSock32.Lib" \
	"$(INTDIR)\Utilities.obj" \
	"$(INTDIR)\ByteBuffer.obj" \
	"$(INTDIR)\CommonObject.obj" \
	"$(INTDIR)\Directory.obj" \
	"$(INTDIR)\File.obj" \
	"$(INTDIR)\FilePath.obj" \
	"$(INTDIR)\FileSystem.obj" \
	"$(INTDIR)\Node.obj" \
	"$(INTDIR)\NodeFinder.obj" \
	"$(INTDIR)\Platform.obj" \
	"$(INTDIR)\PortTime.obj" \
	"$(INTDIR)\ProgramId.obj" \
	"$(INTDIR)\StdDateTime.obj" \
	"$(INTDIR)\stdnoj_global.obj" \
	"$(INTDIR)\StdPath.obj" \
	"$(INTDIR)\StdString.obj" \
	"$(INTDIR)\Url.obj" \
	"$(INTDIR)\SearchData.obj" \
	"$(INTDIR)\Base64.obj" \
	"$(INTDIR)\Email.obj" \
	"$(INTDIR)\EmailFilter.obj" \
	"$(INTDIR)\EMailHeader.obj" \
	"$(INTDIR)\EMailLines.obj" \
	"$(INTDIR)\HttpLogParser.obj" \
	"$(INTDIR)\HyperText01.obj" \
	"$(INTDIR)\InetRfc.obj" \
	"$(INTDIR)\OutboundEmail.obj" \
	"$(INTDIR)\RfcObjects.obj" \
	"$(INTDIR)\Http.obj" \
	"$(INTDIR)\Socket.obj" \
	"$(INTDIR)\SocketMonitor.obj" \
	"$(INTDIR)\SocketSelect.obj" \
	"$(INTDIR)\StdSocket.obj" \
	"$(INTDIR)\EMailFactory.obj"

".\EPOP.EXE" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("MailBot01.dep")
!INCLUDE "MailBot01.dep"
!ELSE 
!MESSAGE Warning: cannot find "MailBot01.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "MailBot01 - Win32 Release" || "$(CFG)" == "MailBot01 - Win32 Debug"
SOURCE=..\..\server\Base64.cpp

"$(INTDIR)\Base64.obj"	"$(INTDIR)\Base64.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\ByteBuffer.cpp

"$(INTDIR)\ByteBuffer.obj"	"$(INTDIR)\ByteBuffer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\CommonObject.cpp

"$(INTDIR)\CommonObject.obj"	"$(INTDIR)\CommonObject.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\Directory.cpp

"$(INTDIR)\Directory.obj"	"$(INTDIR)\Directory.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\Email.cpp

"$(INTDIR)\Email.obj"	"$(INTDIR)\Email.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\EMailFactory.cpp

"$(INTDIR)\EMailFactory.obj"	"$(INTDIR)\EMailFactory.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\EmailFilter.cpp

"$(INTDIR)\EmailFilter.obj"	"$(INTDIR)\EmailFilter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\EMailHeader.cpp

"$(INTDIR)\EMailHeader.obj"	"$(INTDIR)\EMailHeader.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\EMailLines.cpp

"$(INTDIR)\EMailLines.obj"	"$(INTDIR)\EMailLines.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\File.cpp

"$(INTDIR)\File.obj"	"$(INTDIR)\File.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\FilePath.cpp

"$(INTDIR)\FilePath.obj"	"$(INTDIR)\FilePath.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\FileSystem.cpp

"$(INTDIR)\FileSystem.obj"	"$(INTDIR)\FileSystem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\stdnet\Http.cpp

"$(INTDIR)\Http.obj"	"$(INTDIR)\Http.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\HttpLogParser.cpp

"$(INTDIR)\HttpLogParser.obj"	"$(INTDIR)\HttpLogParser.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\HyperText01.cpp

"$(INTDIR)\HyperText01.obj"	"$(INTDIR)\HyperText01.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\InetRfc.cpp

"$(INTDIR)\InetRfc.obj"	"$(INTDIR)\InetRfc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\MailBot01.cpp

"$(INTDIR)\MailBot01.obj"	"$(INTDIR)\MailBot01.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.cpp

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\core\Node.cpp

"$(INTDIR)\Node.obj"	"$(INTDIR)\Node.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\NodeFinder.cpp

"$(INTDIR)\NodeFinder.obj"	"$(INTDIR)\NodeFinder.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\OutboundEmail.cpp

"$(INTDIR)\OutboundEmail.obj"	"$(INTDIR)\OutboundEmail.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\Platform.cpp

"$(INTDIR)\Platform.obj"	"$(INTDIR)\Platform.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\PortTime.cpp

"$(INTDIR)\PortTime.obj"	"$(INTDIR)\PortTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\ProgramId.cpp

"$(INTDIR)\ProgramId.obj"	"$(INTDIR)\ProgramId.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\RfcObjects.cpp

"$(INTDIR)\RfcObjects.obj"	"$(INTDIR)\RfcObjects.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\server\SearchData.cpp

"$(INTDIR)\SearchData.obj"	"$(INTDIR)\SearchData.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\stdnet\Socket.cpp

"$(INTDIR)\Socket.obj"	"$(INTDIR)\Socket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\stdnet\SocketMonitor.cpp

"$(INTDIR)\SocketMonitor.obj"	"$(INTDIR)\SocketMonitor.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\stdnet\SocketSelect.cpp

"$(INTDIR)\SocketSelect.obj"	"$(INTDIR)\SocketSelect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\StdDateTime.cpp

"$(INTDIR)\StdDateTime.obj"	"$(INTDIR)\StdDateTime.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\stdnoj_global.cpp

"$(INTDIR)\stdnoj_global.obj"	"$(INTDIR)\stdnoj_global.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\StdPath.cpp

"$(INTDIR)\StdPath.obj"	"$(INTDIR)\StdPath.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\sockets\StdSocket.cpp

"$(INTDIR)\StdSocket.obj"	"$(INTDIR)\StdSocket.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\StdString.cpp

"$(INTDIR)\StdString.obj"	"$(INTDIR)\StdString.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\Url.cpp

"$(INTDIR)\Url.obj"	"$(INTDIR)\Url.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\core\Utilities.cpp

"$(INTDIR)\Utilities.obj"	"$(INTDIR)\Utilities.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

