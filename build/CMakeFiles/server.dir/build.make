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
CMAKE_SOURCE_DIR = /opt/rc/project/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/rc/project/server/build

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/server/log/log.cc.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/server/log/log.cc.o: ../server/log/log.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/opt/rc/project/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/server/log/log.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server/log/log.cc.o -c /opt/rc/project/server/server/log/log.cc

CMakeFiles/server.dir/server/log/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server/log/log.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /opt/rc/project/server/server/log/log.cc > CMakeFiles/server.dir/server/log/log.cc.i

CMakeFiles/server.dir/server/log/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server/log/log.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /opt/rc/project/server/server/log/log.cc -o CMakeFiles/server.dir/server/log/log.cc.s

CMakeFiles/server.dir/server/log/log.cc.o.requires:

.PHONY : CMakeFiles/server.dir/server/log/log.cc.o.requires

CMakeFiles/server.dir/server/log/log.cc.o.provides: CMakeFiles/server.dir/server/log/log.cc.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server/log/log.cc.o.provides.build
.PHONY : CMakeFiles/server.dir/server/log/log.cc.o.provides

CMakeFiles/server.dir/server/log/log.cc.o.provides.build: CMakeFiles/server.dir/server/log/log.cc.o


CMakeFiles/server.dir/server/utils/utils.cc.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/server/utils/utils.cc.o: ../server/utils/utils.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/opt/rc/project/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server.dir/server/utils/utils.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server/utils/utils.cc.o -c /opt/rc/project/server/server/utils/utils.cc

CMakeFiles/server.dir/server/utils/utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server/utils/utils.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /opt/rc/project/server/server/utils/utils.cc > CMakeFiles/server.dir/server/utils/utils.cc.i

CMakeFiles/server.dir/server/utils/utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server/utils/utils.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /opt/rc/project/server/server/utils/utils.cc -o CMakeFiles/server.dir/server/utils/utils.cc.s

CMakeFiles/server.dir/server/utils/utils.cc.o.requires:

.PHONY : CMakeFiles/server.dir/server/utils/utils.cc.o.requires

CMakeFiles/server.dir/server/utils/utils.cc.o.provides: CMakeFiles/server.dir/server/utils/utils.cc.o.requires
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server/utils/utils.cc.o.provides.build
.PHONY : CMakeFiles/server.dir/server/utils/utils.cc.o.provides

CMakeFiles/server.dir/server/utils/utils.cc.o.provides.build: CMakeFiles/server.dir/server/utils/utils.cc.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server/log/log.cc.o" \
"CMakeFiles/server.dir/server/utils/utils.cc.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../lib/libserver.so: CMakeFiles/server.dir/server/log/log.cc.o
../lib/libserver.so: CMakeFiles/server.dir/server/utils/utils.cc.o
../lib/libserver.so: CMakeFiles/server.dir/build.make
../lib/libserver.so: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/opt/rc/project/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../lib/libserver.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: ../lib/libserver.so

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/requires: CMakeFiles/server.dir/server/log/log.cc.o.requires
CMakeFiles/server.dir/requires: CMakeFiles/server.dir/server/utils/utils.cc.o.requires

.PHONY : CMakeFiles/server.dir/requires

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /opt/rc/project/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/rc/project/server /opt/rc/project/server /opt/rc/project/server/build /opt/rc/project/server/build /opt/rc/project/server/build/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend
