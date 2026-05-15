# Makefile for CMake project with intelligent configuration

# Default target
all: build/build.ninja 
	cmake --build build

# Create build directory and generate build files if needed
build/build.ninja : CMakeLists.txt
	cmake -S . -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=../../nanopi.cmake

# Clean build directory
clean:
	rm -rf build

# Rebuild from scratch
rebuild: clean all

# Phony targets (targets that don't represent files)
.PHONY: all clean rebuild