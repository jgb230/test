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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/jgb/c++/NlsSdkCpp3.X

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jgb/c++/NlsSdkCpp3.X/build

# Include any dependencies generated for this target.
include CMakeFiles/syLongDemo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/syLongDemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/syLongDemo.dir/flags.make

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o: CMakeFiles/syLongDemo.dir/flags.make
CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o: ../demo/speechLongSynthesizerDemo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/c++/NlsSdkCpp3.X/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o -c /home/jgb/c++/NlsSdkCpp3.X/demo/speechLongSynthesizerDemo.cpp

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/c++/NlsSdkCpp3.X/demo/speechLongSynthesizerDemo.cpp > CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.i

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/c++/NlsSdkCpp3.X/demo/speechLongSynthesizerDemo.cpp -o CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.s

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.requires:

.PHONY : CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.requires

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.provides: CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.requires
	$(MAKE) -f CMakeFiles/syLongDemo.dir/build.make CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.provides.build
.PHONY : CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.provides

CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.provides.build: CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o


# Object files for target syLongDemo
syLongDemo_OBJECTS = \
"CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o"

# External object files for target syLongDemo
syLongDemo_EXTERNAL_OBJECTS =

../demo/syLongDemo: CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o
../demo/syLongDemo: CMakeFiles/syLongDemo.dir/build.make
../demo/syLongDemo: CMakeFiles/syLongDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgb/c++/NlsSdkCpp3.X/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../demo/syLongDemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/syLongDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/syLongDemo.dir/build: ../demo/syLongDemo

.PHONY : CMakeFiles/syLongDemo.dir/build

CMakeFiles/syLongDemo.dir/requires: CMakeFiles/syLongDemo.dir/demo/speechLongSynthesizerDemo.cpp.o.requires

.PHONY : CMakeFiles/syLongDemo.dir/requires

CMakeFiles/syLongDemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/syLongDemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/syLongDemo.dir/clean

CMakeFiles/syLongDemo.dir/depend:
	cd /home/jgb/c++/NlsSdkCpp3.X/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgb/c++/NlsSdkCpp3.X /home/jgb/c++/NlsSdkCpp3.X /home/jgb/c++/NlsSdkCpp3.X/build /home/jgb/c++/NlsSdkCpp3.X/build /home/jgb/c++/NlsSdkCpp3.X/build/CMakeFiles/syLongDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/syLongDemo.dir/depend

