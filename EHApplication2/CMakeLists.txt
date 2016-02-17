cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_COMPILER /usr/bin/g++ )
set( CMAKE_C_COMPILER /usr/bin/gcc )

project( EHApplication )

#set( CMAKE_BUILD_TYPE Release )
set( CMAKE_BUILD_TYPE Debug )

#set( BUILD_SHARED_LIBS ON )

add_definitions( -DEH_NO_LODEPNG )
#add_definitions( -DGLEW_STATIC )
#add_definitions( -DNDEBUG )

set( DEFAULT_FLAGS "-std=c++14 -Wall" )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEFAULT_FLAGS}" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEFAULT_FLAGS} -O0" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${DEFAULT_FLAGS} -O2 -DNDEBUG" )

set( SOURCE_ROOT_DIR "~/Documents/Workspace/EHApplication2" )
set( GL_ROOT_DIR "~/Documents/Workspace/GLWrapper" )

list( APPEND SOURCES
    ${GL_ROOT_DIR}/gl_core_3_3.c
    ${GL_ROOT_DIR}/Debug.cpp
    )
list( APPEND SOURCES
    ${SOURCE_ROOT_DIR}/EHApplication.cpp
    )

# plz add lodepng source here

add_executable( EHApplication ${SOURCES} main.cpp )

target_link_libraries( EHApplication glfw GL )
