cmake_minimum_required(VERSION 2.8.9)

SET( CMAKE_CXX_FLAGS "-std=c++11" )

# libraries
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIR} )

set( PROJECT_LINK_LIBS libGLEW.dylib libglfw3.dylib libSOIL.dylib )
link_directories( /usr/local/lib )
include_directories( /usr/local/include )

# executables
add_executable( main main.cpp )
target_link_libraries( main ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} )


add_executable( ex01 ex01.cpp )
target_link_libraries( ex01 ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} )

add_executable( ex02 ex02.cpp )
target_link_libraries( ex02 ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} )

add_executable( ex03 ex03.cpp )
target_link_libraries( ex03 ${PROJECT_LINK_LIBS} ${OPENGL_LIBRARIES} )
