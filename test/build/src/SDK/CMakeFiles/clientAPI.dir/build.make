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
include src/SDK/CMakeFiles/clientAPI.dir/depend.make

# Include the progress variables for this target.
include src/SDK/CMakeFiles/clientAPI.dir/progress.make

# Include the compile flags for this target's objects.
include src/SDK/CMakeFiles/clientAPI.dir/flags.make

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o: src/SDK/CMakeFiles/clientAPI.dir/flags.make
src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o: ../src/SDK/src/clientAPI.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o -c /home/jgb/c++/test/src/SDK/src/clientAPI.cpp

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientAPI.dir/src/clientAPI.cpp.i"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/c++/test/src/SDK/src/clientAPI.cpp > CMakeFiles/clientAPI.dir/src/clientAPI.cpp.i

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientAPI.dir/src/clientAPI.cpp.s"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/c++/test/src/SDK/src/clientAPI.cpp -o CMakeFiles/clientAPI.dir/src/clientAPI.cpp.s

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.requires:

.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.requires

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.provides: src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.requires
	$(MAKE) -f src/SDK/CMakeFiles/clientAPI.dir/build.make src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.provides.build
.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.provides

src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.provides.build: src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o


src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o: src/SDK/CMakeFiles/clientAPI.dir/flags.make
src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o: ../src/SDK/src/clientMsg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o -c /home/jgb/c++/test/src/SDK/src/clientMsg.cpp

src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientAPI.dir/src/clientMsg.cpp.i"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/c++/test/src/SDK/src/clientMsg.cpp > CMakeFiles/clientAPI.dir/src/clientMsg.cpp.i

src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientAPI.dir/src/clientMsg.cpp.s"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/c++/test/src/SDK/src/clientMsg.cpp -o CMakeFiles/clientAPI.dir/src/clientMsg.cpp.s

src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.requires:

.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.requires

src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.provides: src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.requires
	$(MAKE) -f src/SDK/CMakeFiles/clientAPI.dir/build.make src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.provides.build
.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.provides

src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.provides.build: src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o


src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o: src/SDK/CMakeFiles/clientAPI.dir/flags.make
src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o: ../src/SDK/src/md5.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clientAPI.dir/src/md5.cpp.o -c /home/jgb/c++/test/src/SDK/src/md5.cpp

src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clientAPI.dir/src/md5.cpp.i"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jgb/c++/test/src/SDK/src/md5.cpp > CMakeFiles/clientAPI.dir/src/md5.cpp.i

src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clientAPI.dir/src/md5.cpp.s"
	cd /home/jgb/c++/test/build/src/SDK && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jgb/c++/test/src/SDK/src/md5.cpp -o CMakeFiles/clientAPI.dir/src/md5.cpp.s

src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.requires:

.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.requires

src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.provides: src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.requires
	$(MAKE) -f src/SDK/CMakeFiles/clientAPI.dir/build.make src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.provides.build
.PHONY : src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.provides

src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.provides.build: src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o


# Object files for target clientAPI
clientAPI_OBJECTS = \
"CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o" \
"CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o" \
"CMakeFiles/clientAPI.dir/src/md5.cpp.o"

# External object files for target clientAPI
clientAPI_EXTERNAL_OBJECTS =

src/SDK/libclientAPI.so: src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o
src/SDK/libclientAPI.so: src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o
src/SDK/libclientAPI.so: src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o
src/SDK/libclientAPI.so: src/SDK/CMakeFiles/clientAPI.dir/build.make
src/SDK/libclientAPI.so: src/SDK/CMakeFiles/clientAPI.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jgb/c++/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library libclientAPI.so"
	cd /home/jgb/c++/test/build/src/SDK && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clientAPI.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/SDK/CMakeFiles/clientAPI.dir/build: src/SDK/libclientAPI.so

.PHONY : src/SDK/CMakeFiles/clientAPI.dir/build

src/SDK/CMakeFiles/clientAPI.dir/requires: src/SDK/CMakeFiles/clientAPI.dir/src/clientAPI.cpp.o.requires
src/SDK/CMakeFiles/clientAPI.dir/requires: src/SDK/CMakeFiles/clientAPI.dir/src/clientMsg.cpp.o.requires
src/SDK/CMakeFiles/clientAPI.dir/requires: src/SDK/CMakeFiles/clientAPI.dir/src/md5.cpp.o.requires

.PHONY : src/SDK/CMakeFiles/clientAPI.dir/requires

src/SDK/CMakeFiles/clientAPI.dir/clean:
	cd /home/jgb/c++/test/build/src/SDK && $(CMAKE_COMMAND) -P CMakeFiles/clientAPI.dir/cmake_clean.cmake
.PHONY : src/SDK/CMakeFiles/clientAPI.dir/clean

src/SDK/CMakeFiles/clientAPI.dir/depend:
	cd /home/jgb/c++/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jgb/c++/test /home/jgb/c++/test/src/SDK /home/jgb/c++/test/build /home/jgb/c++/test/build/src/SDK /home/jgb/c++/test/build/src/SDK/CMakeFiles/clientAPI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/SDK/CMakeFiles/clientAPI.dir/depend
