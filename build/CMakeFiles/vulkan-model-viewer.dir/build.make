# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Dev/vulkan-model-viewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Dev/vulkan-model-viewer/build

# Include any dependencies generated for this target.
include CMakeFiles/vulkan-model-viewer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/vulkan-model-viewer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vulkan-model-viewer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vulkan-model-viewer.dir/flags.make

CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj: CMakeFiles/vulkan-model-viewer.dir/flags.make
CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj: CMakeFiles/vulkan-model-viewer.dir/includes_CXX.rsp
CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj: C:/Dev/vulkan-model-viewer/src/main.cpp
CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj: CMakeFiles/vulkan-model-viewer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:/Dev/vulkan-model-viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj -MF CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj.d -o CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj -c C:/Dev/vulkan-model-viewer/src/main.cpp

CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.i"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:/Dev/vulkan-model-viewer/src/main.cpp > CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.i

CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.s"
	C:/msys64/mingw64/bin/g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:/Dev/vulkan-model-viewer/src/main.cpp -o CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.s

# Object files for target vulkan-model-viewer
vulkan__model__viewer_OBJECTS = \
"CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj"

# External object files for target vulkan-model-viewer
vulkan__model__viewer_EXTERNAL_OBJECTS =

vulkan-model-viewer.exe: CMakeFiles/vulkan-model-viewer.dir/src/main.cpp.obj
vulkan-model-viewer.exe: CMakeFiles/vulkan-model-viewer.dir/build.make
vulkan-model-viewer.exe: lib/GLFW/src/libglfw3.a
vulkan-model-viewer.exe: C:/VulkanSDK/1.3.243.0/Lib/vulkan-1.lib
vulkan-model-viewer.exe: CMakeFiles/vulkan-model-viewer.dir/linkLibs.rsp
vulkan-model-viewer.exe: CMakeFiles/vulkan-model-viewer.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:/Dev/vulkan-model-viewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable vulkan-model-viewer.exe"
	"C:/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/vulkan-model-viewer.dir/objects.a
	C:/msys64/mingw64/bin/ar.exe qc CMakeFiles/vulkan-model-viewer.dir/objects.a @CMakeFiles/vulkan-model-viewer.dir/objects1.rsp
	C:/msys64/mingw64/bin/g++.exe -g -Wl,--whole-archive CMakeFiles/vulkan-model-viewer.dir/objects.a -Wl,--no-whole-archive -o vulkan-model-viewer.exe -Wl,--out-implib,libvulkan-model-viewer.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/vulkan-model-viewer.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/vulkan-model-viewer.dir/build: vulkan-model-viewer.exe
.PHONY : CMakeFiles/vulkan-model-viewer.dir/build

CMakeFiles/vulkan-model-viewer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vulkan-model-viewer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vulkan-model-viewer.dir/clean

CMakeFiles/vulkan-model-viewer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Dev/vulkan-model-viewer C:/Dev/vulkan-model-viewer C:/Dev/vulkan-model-viewer/build C:/Dev/vulkan-model-viewer/build C:/Dev/vulkan-model-viewer/build/CMakeFiles/vulkan-model-viewer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vulkan-model-viewer.dir/depend

