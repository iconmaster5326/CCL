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
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/base_procs.o \
	${OBJECTDIR}/ccl_type_arg.o \
	${OBJECTDIR}/ccl_type_bool.o \
	${OBJECTDIR}/ccl_type_code.o \
	${OBJECTDIR}/ccl_type_key.o \
	${OBJECTDIR}/ccl_type_list.o \
	${OBJECTDIR}/ccl_type_map.o \
	${OBJECTDIR}/ccl_type_number.o \
	${OBJECTDIR}/ccl_type_string.o \
	${OBJECTDIR}/compiler.o \
	${OBJECTDIR}/debugger.o \
	${OBJECTDIR}/executor.o \
	${OBJECTDIR}/expression.o \
	${OBJECTDIR}/fenv.o \
	${OBJECTDIR}/instruction.o \
	${OBJECTDIR}/lexer.o \
	${OBJECTDIR}/machine.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/object.o \
	${OBJECTDIR}/operation.o \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/proc.o \
	${OBJECTDIR}/proc_hook.o \
	${OBJECTDIR}/proc_user.o \
	${OBJECTDIR}/program.o \
	${OBJECTDIR}/source_range.o \
	${OBJECTDIR}/token.o \
	${OBJECTDIR}/type_hooks.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ccl.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ccl.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ccl ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/base_procs.o: base_procs.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/base_procs.o base_procs.cpp

${OBJECTDIR}/ccl_type_arg.o: ccl_type_arg.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_arg.o ccl_type_arg.cpp

${OBJECTDIR}/ccl_type_bool.o: ccl_type_bool.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_bool.o ccl_type_bool.cpp

${OBJECTDIR}/ccl_type_code.o: ccl_type_code.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_code.o ccl_type_code.cpp

${OBJECTDIR}/ccl_type_key.o: ccl_type_key.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_key.o ccl_type_key.cpp

${OBJECTDIR}/ccl_type_list.o: ccl_type_list.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_list.o ccl_type_list.cpp

${OBJECTDIR}/ccl_type_map.o: ccl_type_map.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_map.o ccl_type_map.cpp

${OBJECTDIR}/ccl_type_number.o: ccl_type_number.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_number.o ccl_type_number.cpp

${OBJECTDIR}/ccl_type_string.o: ccl_type_string.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccl_type_string.o ccl_type_string.cpp

${OBJECTDIR}/compiler.o: compiler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/compiler.o compiler.cpp

${OBJECTDIR}/debugger.o: debugger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/debugger.o debugger.cpp

${OBJECTDIR}/executor.o: executor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/executor.o executor.cpp

${OBJECTDIR}/expression.o: expression.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/expression.o expression.cpp

${OBJECTDIR}/fenv.o: fenv.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fenv.o fenv.cpp

${OBJECTDIR}/instruction.o: instruction.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/instruction.o instruction.cpp

${OBJECTDIR}/lexer.o: lexer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lexer.o lexer.cpp

${OBJECTDIR}/machine.o: machine.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/machine.o machine.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/object.o: object.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/object.o object.cpp

${OBJECTDIR}/operation.o: operation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/operation.o operation.cpp

${OBJECTDIR}/parser.o: parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.o parser.cpp

${OBJECTDIR}/proc.o: proc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/proc.o proc.cpp

${OBJECTDIR}/proc_hook.o: proc_hook.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/proc_hook.o proc_hook.cpp

${OBJECTDIR}/proc_user.o: proc_user.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/proc_user.o proc_user.cpp

${OBJECTDIR}/program.o: program.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/program.o program.cpp

${OBJECTDIR}/source_range.o: source_range.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/source_range.o source_range.cpp

${OBJECTDIR}/token.o: token.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/token.o token.cpp

${OBJECTDIR}/type_hooks.o: type_hooks.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/type_hooks.o type_hooks.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/ccl.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
