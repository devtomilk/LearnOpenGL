# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.5.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.5.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/larry/Code/learnopengl/02HelloTriangle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/larry/Code/learnopengl/02HelloTriangle/build

# Include any dependencies generated for this target.
include CMakeFiles/ex02.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ex02.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ex02.dir/flags.make

CMakeFiles/ex02.dir/ex02.cpp.o: CMakeFiles/ex02.dir/flags.make
CMakeFiles/ex02.dir/ex02.cpp.o: ../ex02.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/larry/Code/learnopengl/02HelloTriangle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ex02.dir/ex02.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ex02.dir/ex02.cpp.o -c /Users/larry/Code/learnopengl/02HelloTriangle/ex02.cpp

CMakeFiles/ex02.dir/ex02.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ex02.dir/ex02.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/larry/Code/learnopengl/02HelloTriangle/ex02.cpp > CMakeFiles/ex02.dir/ex02.cpp.i

CMakeFiles/ex02.dir/ex02.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ex02.dir/ex02.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/larry/Code/learnopengl/02HelloTriangle/ex02.cpp -o CMakeFiles/ex02.dir/ex02.cpp.s

CMakeFiles/ex02.dir/ex02.cpp.o.requires:

.PHONY : CMakeFiles/ex02.dir/ex02.cpp.o.requires

CMakeFiles/ex02.dir/ex02.cpp.o.provides: CMakeFiles/ex02.dir/ex02.cpp.o.requires
	$(MAKE) -f CMakeFiles/ex02.dir/build.make CMakeFiles/ex02.dir/ex02.cpp.o.provides.build
.PHONY : CMakeFiles/ex02.dir/ex02.cpp.o.provides

CMakeFiles/ex02.dir/ex02.cpp.o.provides.build: CMakeFiles/ex02.dir/ex02.cpp.o


# Object files for target ex02
ex02_OBJECTS = \
"CMakeFiles/ex02.dir/ex02.cpp.o"

# External object files for target ex02
ex02_EXTERNAL_OBJECTS =

ex02: CMakeFiles/ex02.dir/ex02.cpp.o
ex02: CMakeFiles/ex02.dir/build.make
ex02: CMakeFiles/ex02.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/larry/Code/learnopengl/02HelloTriangle/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ex02"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ex02.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ex02.dir/build: ex02

.PHONY : CMakeFiles/ex02.dir/build

CMakeFiles/ex02.dir/requires: CMakeFiles/ex02.dir/ex02.cpp.o.requires

.PHONY : CMakeFiles/ex02.dir/requires

CMakeFiles/ex02.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ex02.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ex02.dir/clean

CMakeFiles/ex02.dir/depend:
	cd /Users/larry/Code/learnopengl/02HelloTriangle/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/larry/Code/learnopengl/02HelloTriangle /Users/larry/Code/learnopengl/02HelloTriangle /Users/larry/Code/learnopengl/02HelloTriangle/build /Users/larry/Code/learnopengl/02HelloTriangle/build /Users/larry/Code/learnopengl/02HelloTriangle/build/CMakeFiles/ex02.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ex02.dir/depend

