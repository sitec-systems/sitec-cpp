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
CMAKE_SOURCE_DIR = /home/rd7/work/c_cpp/sitec-c-cpp/can

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rd7/work/c_cpp/sitec-c-cpp/can/build

# Include any dependencies generated for this target.
include CMakeFiles/libcan.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libcan.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libcan.dir/flags.make

CMakeFiles/libcan.dir/src/can.cpp.o: CMakeFiles/libcan.dir/flags.make
CMakeFiles/libcan.dir/src/can.cpp.o: ../src/can.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rd7/work/c_cpp/sitec-c-cpp/can/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/libcan.dir/src/can.cpp.o"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcan.dir/src/can.cpp.o -c /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can.cpp

CMakeFiles/libcan.dir/src/can.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcan.dir/src/can.cpp.i"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can.cpp > CMakeFiles/libcan.dir/src/can.cpp.i

CMakeFiles/libcan.dir/src/can.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcan.dir/src/can.cpp.s"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can.cpp -o CMakeFiles/libcan.dir/src/can.cpp.s

CMakeFiles/libcan.dir/src/can.cpp.o.requires:

.PHONY : CMakeFiles/libcan.dir/src/can.cpp.o.requires

CMakeFiles/libcan.dir/src/can.cpp.o.provides: CMakeFiles/libcan.dir/src/can.cpp.o.requires
	$(MAKE) -f CMakeFiles/libcan.dir/build.make CMakeFiles/libcan.dir/src/can.cpp.o.provides.build
.PHONY : CMakeFiles/libcan.dir/src/can.cpp.o.provides

CMakeFiles/libcan.dir/src/can.cpp.o.provides.build: CMakeFiles/libcan.dir/src/can.cpp.o


CMakeFiles/libcan.dir/src/can_frame.cpp.o: CMakeFiles/libcan.dir/flags.make
CMakeFiles/libcan.dir/src/can_frame.cpp.o: ../src/can_frame.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rd7/work/c_cpp/sitec-c-cpp/can/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/libcan.dir/src/can_frame.cpp.o"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcan.dir/src/can_frame.cpp.o -c /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_frame.cpp

CMakeFiles/libcan.dir/src/can_frame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcan.dir/src/can_frame.cpp.i"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_frame.cpp > CMakeFiles/libcan.dir/src/can_frame.cpp.i

CMakeFiles/libcan.dir/src/can_frame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcan.dir/src/can_frame.cpp.s"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_frame.cpp -o CMakeFiles/libcan.dir/src/can_frame.cpp.s

CMakeFiles/libcan.dir/src/can_frame.cpp.o.requires:

.PHONY : CMakeFiles/libcan.dir/src/can_frame.cpp.o.requires

CMakeFiles/libcan.dir/src/can_frame.cpp.o.provides: CMakeFiles/libcan.dir/src/can_frame.cpp.o.requires
	$(MAKE) -f CMakeFiles/libcan.dir/build.make CMakeFiles/libcan.dir/src/can_frame.cpp.o.provides.build
.PHONY : CMakeFiles/libcan.dir/src/can_frame.cpp.o.provides

CMakeFiles/libcan.dir/src/can_frame.cpp.o.provides.build: CMakeFiles/libcan.dir/src/can_frame.cpp.o


CMakeFiles/libcan.dir/src/can_filter.cpp.o: CMakeFiles/libcan.dir/flags.make
CMakeFiles/libcan.dir/src/can_filter.cpp.o: ../src/can_filter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rd7/work/c_cpp/sitec-c-cpp/can/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/libcan.dir/src/can_filter.cpp.o"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libcan.dir/src/can_filter.cpp.o -c /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_filter.cpp

CMakeFiles/libcan.dir/src/can_filter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libcan.dir/src/can_filter.cpp.i"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_filter.cpp > CMakeFiles/libcan.dir/src/can_filter.cpp.i

CMakeFiles/libcan.dir/src/can_filter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libcan.dir/src/can_filter.cpp.s"
	/opt/poky/2.1/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++   -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/poky/2.1/sysroots/cortexa9hf-neon-poky-linux-gnueabi $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rd7/work/c_cpp/sitec-c-cpp/can/src/can_filter.cpp -o CMakeFiles/libcan.dir/src/can_filter.cpp.s

CMakeFiles/libcan.dir/src/can_filter.cpp.o.requires:

.PHONY : CMakeFiles/libcan.dir/src/can_filter.cpp.o.requires

CMakeFiles/libcan.dir/src/can_filter.cpp.o.provides: CMakeFiles/libcan.dir/src/can_filter.cpp.o.requires
	$(MAKE) -f CMakeFiles/libcan.dir/build.make CMakeFiles/libcan.dir/src/can_filter.cpp.o.provides.build
.PHONY : CMakeFiles/libcan.dir/src/can_filter.cpp.o.provides

CMakeFiles/libcan.dir/src/can_filter.cpp.o.provides.build: CMakeFiles/libcan.dir/src/can_filter.cpp.o


# Object files for target libcan
libcan_OBJECTS = \
"CMakeFiles/libcan.dir/src/can.cpp.o" \
"CMakeFiles/libcan.dir/src/can_frame.cpp.o" \
"CMakeFiles/libcan.dir/src/can_filter.cpp.o"

# External object files for target libcan
libcan_EXTERNAL_OBJECTS =

liblibcan.a: CMakeFiles/libcan.dir/src/can.cpp.o
liblibcan.a: CMakeFiles/libcan.dir/src/can_frame.cpp.o
liblibcan.a: CMakeFiles/libcan.dir/src/can_filter.cpp.o
liblibcan.a: CMakeFiles/libcan.dir/build.make
liblibcan.a: CMakeFiles/libcan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rd7/work/c_cpp/sitec-c-cpp/can/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library liblibcan.a"
	$(CMAKE_COMMAND) -P CMakeFiles/libcan.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libcan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libcan.dir/build: liblibcan.a

.PHONY : CMakeFiles/libcan.dir/build

CMakeFiles/libcan.dir/requires: CMakeFiles/libcan.dir/src/can.cpp.o.requires
CMakeFiles/libcan.dir/requires: CMakeFiles/libcan.dir/src/can_frame.cpp.o.requires
CMakeFiles/libcan.dir/requires: CMakeFiles/libcan.dir/src/can_filter.cpp.o.requires

.PHONY : CMakeFiles/libcan.dir/requires

CMakeFiles/libcan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libcan.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libcan.dir/clean

CMakeFiles/libcan.dir/depend:
	cd /home/rd7/work/c_cpp/sitec-c-cpp/can/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rd7/work/c_cpp/sitec-c-cpp/can /home/rd7/work/c_cpp/sitec-c-cpp/can /home/rd7/work/c_cpp/sitec-c-cpp/can/build /home/rd7/work/c_cpp/sitec-c-cpp/can/build /home/rd7/work/c_cpp/sitec-c-cpp/can/build/CMakeFiles/libcan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libcan.dir/depend

