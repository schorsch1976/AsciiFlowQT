cmake_minimum_required(VERSION 3.7)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

PROJECT(AsciiFlowQT CXX)

############################################################################
# CMake Policies used
############################################################################
IF(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
	CMAKE_POLICY(SET CMP0005 NEW)

	# Work Around Policy CMP0020 is not set:
	# Automatically link Qt executables to qtmain
	CMAKE_POLICY(SET CMP0020 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################
# OS
######################################################################
IF (UNIX)
	MESSAGE(STATUS "This is UNIX")
	ADD_DEFINITIONS(-DUNIX)
	ADD_DEFINITIONS(-DNOEXCEPT=noexcept)

	############################################################################
	# CPP FLAGS
	############################################################################
	SET(CMAKE_CXX_FLAGS   "-Wall ${CMAKE_CXX_FLAGS} -Werror=switch")

	############################################################################
	# RPATH
	# https://cmake.org/Wiki/CMake_RPATH_handling
	############################################################################
	# use, i.e. don't skip the full RPATH for the build tree
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

	# when building, don't use the install RPATH already
	# (but later on when installing)
	SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

	SET(CMAKE_INSTALL_RPATH "${TARGET_LIB_PATH}")

	# add the automatically determined parts of the RPATH
	# which point to directories outside the build tree to the install RPATH
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


	# the RPATH to be used when installing, but only if it's not a system directory
	LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${TARGET_LIB_PATH}" isSystemDir)
	IF("${isSystemDir}" STREQUAL "-1")
		SET(CMAKE_INSTALL_RPATH "${TARGET_LIB_PATH}")
	ENDIF("${isSystemDir}" STREQUAL "-1")

	IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        MESSAGE(STATUS "This is Linux")
		ADD_DEFINITIONS(-DLINUX)

		SET(LINUX TRUE)
		SET(FREEBSD FALSE)
		SET(DARWIN FALSE)

	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	IF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        MESSAGE(STATUS "This is FreeBSD")
		ADD_DEFINITIONS(-DFREEBSD)

		SET(LINUX FALSE)
		SET(FREEBSD TRUE)
		SET(DARWIN FALSE)

	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
	IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        MESSAGE(STATUS "This is Darwin")
		ADD_DEFINITIONS(-DDARWIN)

        SET(LINUX FALSE)
		SET(FREEBSD FALSE)
		SET(DARWIN TRUE)

	ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
ENDIF (UNIX)

IF (WIN32)
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
	IF (MSVC)
		MESSAGE(STATUS "This is Windows: Microsoft Visual Studio")

		ADD_DEFINITIONS(-D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING)
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)

		# C4251 is just noise
		# https://stackoverflow.com/questions/24511376/how-to-dllexport-a-class-derived-from-stdruntime-error/24512237#24512237
		# disable C4251
		add_compile_options(/wd4251)
		add_compile_options(/wd4275)

		# noexcept
		IF (MSVC_TOOLSET_VERSION VERSION_GREATER_EQUAL 140)
			MESSAGE(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION} : noexcept supported")
			ADD_DEFINITIONS(-DNOEXCEPT=noexcept )
		ELSE (MSVC_TOOLSET_VERSION VERSION_GREATER_EQUAL 140)
			MESSAGE(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION} : noexcept not supported")
			ADD_DEFINITIONS(-DNOEXCEPT= )
		ENDIF (MSVC_TOOLSET_VERSION VERSION_GREATER_EQUAL 140)

	ENDIF (MSVC)

	IF(MSYS)
		MESSAGE(STATUS "This is Windows: MSYS")
		ADD_DEFINITIONS(-DNOEXCEPT=noexcept )
		SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations)
	ENDIF(MSYS)
ENDIF (WIN32)

######################################################################
# Version
######################################################################
# branch
EXECUTE_PROCESS(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# commit hash
EXECUTE_PROCESS(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# description
EXECUTE_PROCESS(
  COMMAND git describe --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_DESCRIPTION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

ADD_DEFINITIONS("-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}")
ADD_DEFINITIONS("-DGIT_BRANCH=${GIT_BRANCH}")
ADD_DEFINITIONS("-DGIT_DESCRIPTION=${GIT_DESCRIPTION}")

######################################################################
# QT 5
######################################################################
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

FIND_PACKAGE(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

######################################################################
# Subdirs
######################################################################

ADD_SUBDIRECTORY(src)
