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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jgb/b_test/cc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgb/b_test/cc/build

# Include any dependencies generated for this target.
include CMakeFiles/jgb_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/jgb_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jgb_test.dir/flags.make

CMakeFiles/jgb_test.dir/src/test.cpp.o: CMakeFiles/jgb_test.dir/flags.make
CMakeFiles/jgb_test.dir/src/test.cpp.o: ../src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/b_test/cc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/jgb_test.dir/src/test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jgb_test.dir/src/test.cpp.o -c /home/jgb/b_test/cc/src/test.cpp

CMakeFiles/jgb_test.dir/src/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jgb_test.dir/src/test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/b_test/cc/src/test.cpp > CMakeFiles/jgb_test.dir/src/test.cpp.i

CMakeFiles/jgb_test.dir/src/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jgb_test.dir/src/test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/b_test/cc/src/test.cpp -o CMakeFiles/jgb_test.dir/src/test.cpp.s

CMakeFiles/jgb_test.dir/src/test.cpp.o.requires:

.PHONY : CMakeFiles/jgb_test.dir/src/test.cpp.o.requires

CMakeFiles/jgb_test.dir/src/test.cpp.o.provides: CMakeFiles/jgb_test.dir/src/test.cpp.o.requires
	$(MAKE) -f CMakeFiles/jgb_test.dir/build.make CMakeFiles/jgb_test.dir/src/test.cpp.o.provides.build
.PHONY : CMakeFiles/jgb_test.dir/src/test.cpp.o.provides

CMakeFiles/jgb_test.dir/src/test.cpp.o.provides.build: CMakeFiles/jgb_test.dir/src/test.cpp.o


# Object files for target jgb_test
jgb_test_OBJECTS = \
"CMakeFiles/jgb_test.dir/src/test.cpp.o"

# External object files for target jgb_test
jgb_test_EXTERNAL_OBJECTS =

jgb_test: CMakeFiles/jgb_test.dir/src/test.cpp.o
jgb_test: CMakeFiles/jgb_test.dir/build.make
jgb_test: CMakeFiles/jgb_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgb/b_test/cc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable jgb_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jgb_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jgb_test.dir/build: jgb_test

.PHONY : CMakeFiles/jgb_test.dir/build

CMakeFiles/jgb_test.dir/requires: CMakeFiles/jgb_test.dir/src/test.cpp.o.requires

.PHONY : CMakeFiles/jgb_test.dir/requires

CMakeFiles/jgb_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jgb_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jgb_test.dir/clean

CMakeFiles/jgb_test.dir/depend:
	cd /home/jgb/b_test/cc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgb/b_test/cc /home/jgb/b_test/cc /home/jgb/b_test/cc/build /home/jgb/b_test/cc/build /home/jgb/b_test/cc/build/CMakeFiles/jgb_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/jgb_test.dir/depend

