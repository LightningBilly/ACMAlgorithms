# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/bytedance/CLionProjects/glTriangle

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/bytedance/CLionProjects/glTriangle/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/glTriangle.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glTriangle.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glTriangle.dir/flags.make

CMakeFiles/glTriangle.dir/main.cpp.o: CMakeFiles/glTriangle.dir/flags.make
CMakeFiles/glTriangle.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/CLionProjects/glTriangle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/glTriangle.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/glTriangle.dir/main.cpp.o -c /Users/bytedance/CLionProjects/glTriangle/main.cpp

CMakeFiles/glTriangle.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glTriangle.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/CLionProjects/glTriangle/main.cpp > CMakeFiles/glTriangle.dir/main.cpp.i

CMakeFiles/glTriangle.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glTriangle.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/CLionProjects/glTriangle/main.cpp -o CMakeFiles/glTriangle.dir/main.cpp.s

CMakeFiles/glTriangle.dir/PolyMesh.cpp.o: CMakeFiles/glTriangle.dir/flags.make
CMakeFiles/glTriangle.dir/PolyMesh.cpp.o: ../PolyMesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/CLionProjects/glTriangle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/glTriangle.dir/PolyMesh.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/glTriangle.dir/PolyMesh.cpp.o -c /Users/bytedance/CLionProjects/glTriangle/PolyMesh.cpp

CMakeFiles/glTriangle.dir/PolyMesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glTriangle.dir/PolyMesh.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/CLionProjects/glTriangle/PolyMesh.cpp > CMakeFiles/glTriangle.dir/PolyMesh.cpp.i

CMakeFiles/glTriangle.dir/PolyMesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glTriangle.dir/PolyMesh.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/CLionProjects/glTriangle/PolyMesh.cpp -o CMakeFiles/glTriangle.dir/PolyMesh.cpp.s

CMakeFiles/glTriangle.dir/IOManager.cpp.o: CMakeFiles/glTriangle.dir/flags.make
CMakeFiles/glTriangle.dir/IOManager.cpp.o: ../IOManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/bytedance/CLionProjects/glTriangle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/glTriangle.dir/IOManager.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/glTriangle.dir/IOManager.cpp.o -c /Users/bytedance/CLionProjects/glTriangle/IOManager.cpp

CMakeFiles/glTriangle.dir/IOManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glTriangle.dir/IOManager.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/bytedance/CLionProjects/glTriangle/IOManager.cpp > CMakeFiles/glTriangle.dir/IOManager.cpp.i

CMakeFiles/glTriangle.dir/IOManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glTriangle.dir/IOManager.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/bytedance/CLionProjects/glTriangle/IOManager.cpp -o CMakeFiles/glTriangle.dir/IOManager.cpp.s

# Object files for target glTriangle
glTriangle_OBJECTS = \
"CMakeFiles/glTriangle.dir/main.cpp.o" \
"CMakeFiles/glTriangle.dir/PolyMesh.cpp.o" \
"CMakeFiles/glTriangle.dir/IOManager.cpp.o"

# External object files for target glTriangle
glTriangle_EXTERNAL_OBJECTS =

glTriangle: CMakeFiles/glTriangle.dir/main.cpp.o
glTriangle: CMakeFiles/glTriangle.dir/PolyMesh.cpp.o
glTriangle: CMakeFiles/glTriangle.dir/IOManager.cpp.o
glTriangle: CMakeFiles/glTriangle.dir/build.make
glTriangle: /Library/Developer/CommandLineTools/usr/include/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib
glTriangle: /Library/Developer/CommandLineTools/usr/include/glfw/3.3.4/lib/libglfw.3.3.dylib
glTriangle: CMakeFiles/glTriangle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/bytedance/CLionProjects/glTriangle/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable glTriangle"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glTriangle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glTriangle.dir/build: glTriangle

.PHONY : CMakeFiles/glTriangle.dir/build

CMakeFiles/glTriangle.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glTriangle.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glTriangle.dir/clean

CMakeFiles/glTriangle.dir/depend:
	cd /Users/bytedance/CLionProjects/glTriangle/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/bytedance/CLionProjects/glTriangle /Users/bytedance/CLionProjects/glTriangle /Users/bytedance/CLionProjects/glTriangle/cmake-build-debug /Users/bytedance/CLionProjects/glTriangle/cmake-build-debug /Users/bytedance/CLionProjects/glTriangle/cmake-build-debug/CMakeFiles/glTriangle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glTriangle.dir/depend

