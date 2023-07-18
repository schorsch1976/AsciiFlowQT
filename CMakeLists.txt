cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AsciiFlowQtWX C CXX)

############################################################################
# Own CMake functions
############################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)
include(DevClangFormat)
include(DevClangTidy)
include(GitVersion)
include(OS)

OS()
GitVersion()

option(DEV_CLANG_FORMAT "Run clang-format prior to compilation on source files." OFF)
option(DEV_CLANG_TIDY "Run clang-tidy prior to compilation on source files." OFF)


############################################################################
# CMake Policies used
############################################################################
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 NEW)

	# Work Around Policy CMP0020 is not set:
	cmake_policy(SET CMP0020 NEW)
endif(COMMAND cmake_policy)


######################################################################
# Wx/Qt
######################################################################
option(BUILD_WITH_WX "Build AsciiFlowWx" ON)
option(BUILD_WITH_QT "Build AsciiFlowQt" ON)

######################################################################
# Subdirs
######################################################################
add_subdirectory(3rdParty)
add_subdirectory(src)
add_subdirectory(man)
