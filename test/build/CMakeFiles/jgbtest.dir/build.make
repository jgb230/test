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
CMAKE_SOURCE_DIR = /home/jgb/c++/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgb/c++/test/build

# Include any dependencies generated for this target.
include CMakeFiles/jgbtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/jgbtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jgbtest.dir/flags.make

CMakeFiles/jgbtest.dir/src/main.cpp.o: CMakeFiles/jgbtest.dir/flags.make
CMakeFiles/jgbtest.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/jgbtest.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jgbtest.dir/src/main.cpp.o -c /home/jgb/c++/test/src/main.cpp

CMakeFiles/jgbtest.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jgbtest.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/c++/test/src/main.cpp > CMakeFiles/jgbtest.dir/src/main.cpp.i

CMakeFiles/jgbtest.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jgbtest.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/c++/test/src/main.cpp -o CMakeFiles/jgbtest.dir/src/main.cpp.s

CMakeFiles/jgbtest.dir/src/main.cpp.o.requires:

.PHONY : CMakeFiles/jgbtest.dir/src/main.cpp.o.requires

CMakeFiles/jgbtest.dir/src/main.cpp.o.provides: CMakeFiles/jgbtest.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/jgbtest.dir/build.make CMakeFiles/jgbtest.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/jgbtest.dir/src/main.cpp.o.provides

CMakeFiles/jgbtest.dir/src/main.cpp.o.provides.build: CMakeFiles/jgbtest.dir/src/main.cpp.o


# Object files for target jgbtest
jgbtest_OBJECTS = \
"CMakeFiles/jgbtest.dir/src/main.cpp.o"

# External object files for target jgbtest
jgbtest_EXTERNAL_OBJECTS =

jgbtest: CMakeFiles/jgbtest.dir/src/main.cpp.o
jgbtest: CMakeFiles/jgbtest.dir/build.make
jgbtest: libB.so
jgbtest: libC.so
jgbtest: src/SDK/libclientAPI.so
jgbtest: libA.so
jgbtest: CMakeFiles/jgbtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable jgbtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jgbtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jgbtest.dir/build: jgbtest

.PHONY : CMakeFiles/jgbtest.dir/build

CMakeFiles/jgbtest.dir/requires: CMakeFiles/jgbtest.dir/src/main.cpp.o.requires

.PHONY : CMakeFiles/jgbtest.dir/requires

CMakeFiles/jgbtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jgbtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jgbtest.dir/clean

CMakeFiles/jgbtest.dir/depend:
	cd /home/jgb/c++/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgb/c++/test /home/jgb/c++/test /home/jgb/c++/test/build /home/jgb/c++/test/build /home/jgb/c++/test/build/CMakeFiles/jgbtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/jgbtest.dir/depend

