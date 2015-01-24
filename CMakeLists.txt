# Ver >2.6
cmake_minimum_required (VERSION 2.6)

project (cbasic)

# Flags
SET(CMAKE_CXX_FLAGS "-std=c++0x -ggdb")

# Version
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

# Folders
set (SOURCE_DIR src)
set (BUILD_DIR build)
set (BIN_DIR ${BUILD_DIR}/bin)

# Include dirs
include_directories (
	${SOURCE_DIR}
	)

# Config file
configure_file (
	"${SOURCE_DIR}/configure.h.in"
	"${SOURCE_DIR}/configure.h"
	)

# Header files
set (HEADER_FILES
	${SOURCE_DIR}/configure.h
	)

# Source files
set (SOURCE_FILES
	${SOURCE_DIR}/cbasic.cc
	)
 
# Executables
add_executable (${BIN_DIR}/cbasic
	${SOURCE_FILES}
	${HEADER_FILES}
	)
