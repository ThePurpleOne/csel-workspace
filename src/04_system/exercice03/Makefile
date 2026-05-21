all: build/build.ninja
	cmake --build build

build/build.ninja : CMakeLists.txt
	cmake -S . -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=../../nanopi.cmake

clean:
	rm -rf build

rebuild: clean all

.PHONY: all clean rebuild
