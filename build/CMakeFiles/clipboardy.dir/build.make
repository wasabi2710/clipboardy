# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wasabi/Documents/dev/buildessentials/clipboardy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wasabi/Documents/dev/buildessentials/clipboardy/build

# Include any dependencies generated for this target.
include CMakeFiles/clipboardy.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/clipboardy.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clipboardy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clipboardy.dir/flags.make

CMakeFiles/clipboardy.dir/src/linux.c.o: CMakeFiles/clipboardy.dir/flags.make
CMakeFiles/clipboardy.dir/src/linux.c.o: /home/wasabi/Documents/dev/buildessentials/clipboardy/src/linux.c
CMakeFiles/clipboardy.dir/src/linux.c.o: CMakeFiles/clipboardy.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wasabi/Documents/dev/buildessentials/clipboardy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/clipboardy.dir/src/linux.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clipboardy.dir/src/linux.c.o -MF CMakeFiles/clipboardy.dir/src/linux.c.o.d -o CMakeFiles/clipboardy.dir/src/linux.c.o -c /home/wasabi/Documents/dev/buildessentials/clipboardy/src/linux.c

CMakeFiles/clipboardy.dir/src/linux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/clipboardy.dir/src/linux.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wasabi/Documents/dev/buildessentials/clipboardy/src/linux.c > CMakeFiles/clipboardy.dir/src/linux.c.i

CMakeFiles/clipboardy.dir/src/linux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/clipboardy.dir/src/linux.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wasabi/Documents/dev/buildessentials/clipboardy/src/linux.c -o CMakeFiles/clipboardy.dir/src/linux.c.s

# Object files for target clipboardy
clipboardy_OBJECTS = \
"CMakeFiles/clipboardy.dir/src/linux.c.o"

# External object files for target clipboardy
clipboardy_EXTERNAL_OBJECTS =

clipboardy: CMakeFiles/clipboardy.dir/src/linux.c.o
clipboardy: CMakeFiles/clipboardy.dir/build.make
clipboardy: CMakeFiles/clipboardy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wasabi/Documents/dev/buildessentials/clipboardy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable clipboardy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clipboardy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clipboardy.dir/build: clipboardy
.PHONY : CMakeFiles/clipboardy.dir/build

CMakeFiles/clipboardy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clipboardy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clipboardy.dir/clean

CMakeFiles/clipboardy.dir/depend:
	cd /home/wasabi/Documents/dev/buildessentials/clipboardy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wasabi/Documents/dev/buildessentials/clipboardy /home/wasabi/Documents/dev/buildessentials/clipboardy /home/wasabi/Documents/dev/buildessentials/clipboardy/build /home/wasabi/Documents/dev/buildessentials/clipboardy/build /home/wasabi/Documents/dev/buildessentials/clipboardy/build/CMakeFiles/clipboardy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clipboardy.dir/depend

