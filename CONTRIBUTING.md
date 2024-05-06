# Contributing

I used [CMake](github.com/kitware/cmake) for the cross-platform build system generation.

## Unix instructions

Here is how I compile for development on Unix:

1. `mkdir build && cd build` to create and enter a temporary build directory
2. `cmake .. -DCMAKE_BUILD_TYPE=Debug` to generate the build system for your platform. The `Debug` build type will generate debug information and files for IDE language server integration.
3. `make` to generate the binaries for Unix in the `bin` directory

Use `cd ..` to go back to the root dir and then run with `bin/c-server`.
