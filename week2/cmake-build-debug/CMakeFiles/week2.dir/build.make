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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alex/CLionProjects/CS253-19F/week2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/week2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/week2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/week2.dir/flags.make

CMakeFiles/week2.dir/Five.cpp.o: CMakeFiles/week2.dir/flags.make
CMakeFiles/week2.dir/Five.cpp.o: ../Five.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/week2.dir/Five.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/week2.dir/Five.cpp.o -c /home/alex/CLionProjects/CS253-19F/week2/Five.cpp

CMakeFiles/week2.dir/Five.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/week2.dir/Five.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alex/CLionProjects/CS253-19F/week2/Five.cpp > CMakeFiles/week2.dir/Five.cpp.i

CMakeFiles/week2.dir/Five.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/week2.dir/Five.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alex/CLionProjects/CS253-19F/week2/Five.cpp -o CMakeFiles/week2.dir/Five.cpp.s

# Object files for target week2
week2_OBJECTS = \
"CMakeFiles/week2.dir/Five.cpp.o"

# External object files for target week2
week2_EXTERNAL_OBJECTS =

week2: CMakeFiles/week2.dir/Five.cpp.o
week2: CMakeFiles/week2.dir/build.make
week2: CMakeFiles/week2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable week2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/week2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/week2.dir/build: week2

.PHONY : CMakeFiles/week2.dir/build

CMakeFiles/week2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/week2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/week2.dir/clean

CMakeFiles/week2.dir/depend:
	cd /home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/CLionProjects/CS253-19F/week2 /home/alex/CLionProjects/CS253-19F/week2 /home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug /home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug /home/alex/CLionProjects/CS253-19F/week2/cmake-build-debug/CMakeFiles/week2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/week2.dir/depend

