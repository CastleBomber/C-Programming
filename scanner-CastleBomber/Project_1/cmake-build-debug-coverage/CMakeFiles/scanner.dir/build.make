# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /home/castlebomber/cmake-3.19.4-Linux-x86_64/bin/cmake

# The command to remove a file.
RM = /home/castlebomber/cmake-3.19.4-Linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage

# Include any dependencies generated for this target.
include CMakeFiles/scanner.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/scanner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/scanner.dir/flags.make

CMakeFiles/scanner.dir/src/main.c.o: CMakeFiles/scanner.dir/flags.make
CMakeFiles/scanner.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/scanner.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/scanner.dir/src/main.c.o -c /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/main.c

CMakeFiles/scanner.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/scanner.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/main.c > CMakeFiles/scanner.dir/src/main.c.i

CMakeFiles/scanner.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/scanner.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/main.c -o CMakeFiles/scanner.dir/src/main.c.s

CMakeFiles/scanner.dir/src/scanner.c.o: CMakeFiles/scanner.dir/flags.make
CMakeFiles/scanner.dir/src/scanner.c.o: ../src/scanner.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/scanner.dir/src/scanner.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/scanner.dir/src/scanner.c.o -c /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/scanner.c

CMakeFiles/scanner.dir/src/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/scanner.dir/src/scanner.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/scanner.c > CMakeFiles/scanner.dir/src/scanner.c.i

CMakeFiles/scanner.dir/src/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/scanner.dir/src/scanner.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/src/scanner.c -o CMakeFiles/scanner.dir/src/scanner.c.s

# Object files for target scanner
scanner_OBJECTS = \
"CMakeFiles/scanner.dir/src/main.c.o" \
"CMakeFiles/scanner.dir/src/scanner.c.o"

# External object files for target scanner
scanner_EXTERNAL_OBJECTS =

scanner: CMakeFiles/scanner.dir/src/main.c.o
scanner: CMakeFiles/scanner.dir/src/scanner.c.o
scanner: CMakeFiles/scanner.dir/build.make
scanner: CMakeFiles/scanner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable scanner"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/scanner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/scanner.dir/build: scanner

.PHONY : CMakeFiles/scanner.dir/build

CMakeFiles/scanner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/scanner.dir/cmake_clean.cmake
.PHONY : CMakeFiles/scanner.dir/clean

CMakeFiles/scanner.dir/depend:
	cd /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1 /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1 /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage /mnt/c/Users/CastleBomber/Documents/GitHub/scanner-CastleBomber/Project_1/cmake-build-debug-coverage/CMakeFiles/scanner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/scanner.dir/depend

