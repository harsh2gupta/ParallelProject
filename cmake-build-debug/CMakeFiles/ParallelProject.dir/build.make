# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/ParallelProject.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ParallelProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ParallelProject.dir/flags.make

CMakeFiles/ParallelProject.dir/main.cpp.o: CMakeFiles/ParallelProject.dir/flags.make
CMakeFiles/ParallelProject.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ParallelProject.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ParallelProject.dir/main.cpp.o -c "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/main.cpp"

CMakeFiles/ParallelProject.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ParallelProject.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/main.cpp" > CMakeFiles/ParallelProject.dir/main.cpp.i

CMakeFiles/ParallelProject.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ParallelProject.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/main.cpp" -o CMakeFiles/ParallelProject.dir/main.cpp.s

CMakeFiles/ParallelProject.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/ParallelProject.dir/main.cpp.o.requires

CMakeFiles/ParallelProject.dir/main.cpp.o.provides: CMakeFiles/ParallelProject.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ParallelProject.dir/build.make CMakeFiles/ParallelProject.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/ParallelProject.dir/main.cpp.o.provides

CMakeFiles/ParallelProject.dir/main.cpp.o.provides.build: CMakeFiles/ParallelProject.dir/main.cpp.o


CMakeFiles/ParallelProject.dir/test.cpp.o: CMakeFiles/ParallelProject.dir/flags.make
CMakeFiles/ParallelProject.dir/test.cpp.o: ../test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ParallelProject.dir/test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ParallelProject.dir/test.cpp.o -c "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/test.cpp"

CMakeFiles/ParallelProject.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ParallelProject.dir/test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/test.cpp" > CMakeFiles/ParallelProject.dir/test.cpp.i

CMakeFiles/ParallelProject.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ParallelProject.dir/test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/test.cpp" -o CMakeFiles/ParallelProject.dir/test.cpp.s

CMakeFiles/ParallelProject.dir/test.cpp.o.requires:

.PHONY : CMakeFiles/ParallelProject.dir/test.cpp.o.requires

CMakeFiles/ParallelProject.dir/test.cpp.o.provides: CMakeFiles/ParallelProject.dir/test.cpp.o.requires
	$(MAKE) -f CMakeFiles/ParallelProject.dir/build.make CMakeFiles/ParallelProject.dir/test.cpp.o.provides.build
.PHONY : CMakeFiles/ParallelProject.dir/test.cpp.o.provides

CMakeFiles/ParallelProject.dir/test.cpp.o.provides.build: CMakeFiles/ParallelProject.dir/test.cpp.o


# Object files for target ParallelProject
ParallelProject_OBJECTS = \
"CMakeFiles/ParallelProject.dir/main.cpp.o" \
"CMakeFiles/ParallelProject.dir/test.cpp.o"

# External object files for target ParallelProject
ParallelProject_EXTERNAL_OBJECTS =

ParallelProject: CMakeFiles/ParallelProject.dir/main.cpp.o
ParallelProject: CMakeFiles/ParallelProject.dir/test.cpp.o
ParallelProject: CMakeFiles/ParallelProject.dir/build.make
ParallelProject: CMakeFiles/ParallelProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ParallelProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ParallelProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ParallelProject.dir/build: ParallelProject

.PHONY : CMakeFiles/ParallelProject.dir/build

CMakeFiles/ParallelProject.dir/requires: CMakeFiles/ParallelProject.dir/main.cpp.o.requires
CMakeFiles/ParallelProject.dir/requires: CMakeFiles/ParallelProject.dir/test.cpp.o.requires

.PHONY : CMakeFiles/ParallelProject.dir/requires

CMakeFiles/ParallelProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ParallelProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ParallelProject.dir/clean

CMakeFiles/ParallelProject.dir/depend:
	cd "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject" "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject" "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug" "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug" "/Users/harsh/Documents/StonyBrook/SecondSem/Parallel Prog/shweta/ParallelProject/cmake-build-debug/CMakeFiles/ParallelProject.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ParallelProject.dir/depend

