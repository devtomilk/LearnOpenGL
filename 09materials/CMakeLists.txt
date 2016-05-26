cmake_minimum_required(VERSION 2.8.9)
project(09materials)

SET( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set ( PROJECT_LINK_LIBS libGLEW.dylib libglfw3.dylib libSOIL.dylib )
link_directories( /usr/local/lib )
include_directories( /usr/local/include )

add_executable( main main.cpp )
target_link_libraries( main ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} )
