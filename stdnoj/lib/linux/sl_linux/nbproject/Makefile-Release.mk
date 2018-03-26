#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/32229614/Platform.o \
	${OBJECTDIR}/_ext/32229614/Directory.o \
	${OBJECTDIR}/_ext/32229614/PortTime.o \
	${OBJECTDIR}/_ext/32229614/ProgramId.o \
	${OBJECTDIR}/_ext/32229614/Utilities.o \
	${OBJECTDIR}/_ext/32229614/ByteBuffer.o \
	${OBJECTDIR}/_ext/32229614/Node.o \
	${OBJECTDIR}/_ext/32229614/CommonObject.o \
	${OBJECTDIR}/_ext/32229614/FilePath.o \
	${OBJECTDIR}/_ext/32229614/StdDateTime.o \
	${OBJECTDIR}/_ext/32229614/File.o \
	${OBJECTDIR}/_ext/32229614/StdString.o \
	${OBJECTDIR}/_ext/32229614/Url.o \
	${OBJECTDIR}/_ext/32229614/StdPath.o \
	${OBJECTDIR}/_ext/32229614/NodeFinder.o \
	${OBJECTDIR}/_ext/32229614/FileSystem.o \
	${OBJECTDIR}/_ext/32229614/stdnoj_global.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a

${OBJECTDIR}/_ext/32229614/Platform.o: ../../../core/Platform.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/Platform.o ../../../core/Platform.cpp

${OBJECTDIR}/_ext/32229614/Directory.o: ../../../core/Directory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/Directory.o ../../../core/Directory.cpp

${OBJECTDIR}/_ext/32229614/PortTime.o: ../../../core/PortTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/PortTime.o ../../../core/PortTime.cpp

${OBJECTDIR}/_ext/32229614/ProgramId.o: ../../../core/ProgramId.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/ProgramId.o ../../../core/ProgramId.cpp

${OBJECTDIR}/_ext/32229614/Utilities.o: ../../../core/Utilities.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/Utilities.o ../../../core/Utilities.cpp

${OBJECTDIR}/_ext/32229614/ByteBuffer.o: ../../../core/ByteBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/ByteBuffer.o ../../../core/ByteBuffer.cpp

${OBJECTDIR}/_ext/32229614/Node.o: ../../../core/Node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/Node.o ../../../core/Node.cpp

${OBJECTDIR}/_ext/32229614/CommonObject.o: ../../../core/CommonObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/CommonObject.o ../../../core/CommonObject.cpp

${OBJECTDIR}/_ext/32229614/FilePath.o: ../../../core/FilePath.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/FilePath.o ../../../core/FilePath.cpp

${OBJECTDIR}/_ext/32229614/StdDateTime.o: ../../../core/StdDateTime.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/StdDateTime.o ../../../core/StdDateTime.cpp

${OBJECTDIR}/_ext/32229614/File.o: ../../../core/File.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/File.o ../../../core/File.cpp

${OBJECTDIR}/_ext/32229614/StdString.o: ../../../core/StdString.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/StdString.o ../../../core/StdString.cpp

${OBJECTDIR}/_ext/32229614/Url.o: ../../../core/Url.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/Url.o ../../../core/Url.cpp

${OBJECTDIR}/_ext/32229614/StdPath.o: ../../../core/StdPath.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/StdPath.o ../../../core/StdPath.cpp

${OBJECTDIR}/_ext/32229614/NodeFinder.o: ../../../core/NodeFinder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/NodeFinder.o ../../../core/NodeFinder.cpp

${OBJECTDIR}/_ext/32229614/FileSystem.o: ../../../core/FileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/FileSystem.o ../../../core/FileSystem.cpp

${OBJECTDIR}/_ext/32229614/stdnoj_global.o: ../../../core/stdnoj_global.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/32229614
	${RM} $@.d
	$(COMPILE.cc) -O2 -DUNIX -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/32229614/stdnoj_global.o ../../../core/stdnoj_global.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsl_linux.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
