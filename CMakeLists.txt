cmake_minimum_required (VERSION 2.6)

project (OHMComm)

# Build 'shared libraries' as default setting for command add_library()
SET(BUILD_SHARED_LIBS ON)

# If the compiler is some version of Microsoft Visual C++
if(MSVC)
	set(CMAKE_CXX_FLAGS "/W3 /D_UNICODE /DUNICODE")
	add_definitions(-DWIN32_LEAN_AND_MEAN)
else()
	set(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -pthread")
endif()


# Dependencies

# Set output copy path
add_subdirectory(lib/rtaudio-4.1.1 build/rtaudio)
add_subdirectory(lib/opus-1.1 build/opus)

#Include headers in the project settings (as search-path for header-files)
include_directories ("${PROJECT_SOURCE_DIR}/lib/rtaudio-4.1.1")
include_directories ("${PROJECT_SOURCE_DIR}/lib/opus-1.1/include")

####
# Main files
####
#build all from ./src into ./build
add_subdirectory(src build)
