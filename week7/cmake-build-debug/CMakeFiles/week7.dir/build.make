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
CMAKE_SOURCE_DIR = /home/alex/CLionProjects/CS253-19F/week7

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/week7.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/week7.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/week7.dir/flags.make

CMakeFiles/week7.dir/TwentySix.cpp.o: CMakeFiles/week7.dir/flags.make
CMakeFiles/week7.dir/TwentySix.cpp.o: ../TwentySix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/week7.dir/TwentySix.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/week7.dir/TwentySix.cpp.o -c /home/alex/CLionProjects/CS253-19F/week7/TwentySix.cpp

CMakeFiles/week7.dir/TwentySix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/week7.dir/TwentySix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alex/CLionProjects/CS253-19F/week7/TwentySix.cpp > CMakeFiles/week7.dir/TwentySix.cpp.i

CMakeFiles/week7.dir/TwentySix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/week7.dir/TwentySix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alex/CLionProjects/CS253-19F/week7/TwentySix.cpp -o CMakeFiles/week7.dir/TwentySix.cpp.s

CMakeFiles/week7.dir/TwentySeven.cpp.o: CMakeFiles/week7.dir/flags.make
CMakeFiles/week7.dir/TwentySeven.cpp.o: ../TwentySeven.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/week7.dir/TwentySeven.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/week7.dir/TwentySeven.cpp.o -c /home/alex/CLionProjects/CS253-19F/week7/TwentySeven.cpp

CMakeFiles/week7.dir/TwentySeven.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/week7.dir/TwentySeven.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alex/CLionProjects/CS253-19F/week7/TwentySeven.cpp > CMakeFiles/week7.dir/TwentySeven.cpp.i

CMakeFiles/week7.dir/TwentySeven.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/week7.dir/TwentySeven.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alex/CLionProjects/CS253-19F/week7/TwentySeven.cpp -o CMakeFiles/week7.dir/TwentySeven.cpp.s

# Object files for target week7
week7_OBJECTS = \
"CMakeFiles/week7.dir/TwentySix.cpp.o" \
"CMakeFiles/week7.dir/TwentySeven.cpp.o"

# External object files for target week7
week7_EXTERNAL_OBJECTS =

week7: CMakeFiles/week7.dir/TwentySix.cpp.o
week7: CMakeFiles/week7.dir/TwentySeven.cpp.o
week7: CMakeFiles/week7.dir/build.make
week7: CMakeFiles/week7.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable week7"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/week7.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/week7.dir/build: week7

.PHONY : CMakeFiles/week7.dir/build

CMakeFiles/week7.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/week7.dir/cmake_clean.cmake
.PHONY : CMakeFiles/week7.dir/clean

CMakeFiles/week7.dir/depend:
	cd /home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alex/CLionProjects/CS253-19F/week7 /home/alex/CLionProjects/CS253-19F/week7 /home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug /home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug /home/alex/CLionProjects/CS253-19F/week7/cmake-build-debug/CMakeFiles/week7.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/week7.dir/depend

