# sitec-cpp

Supporting library for sitec systems Linux based systems. This library contains
the following subdirectories:

  * `can`

## Build dependencies

  * C++ 14 compliant compiler

## Build

The libraries are designed as header only libraries. Copy these into your own
project directory and include it.

If you want to build the examples perform the following steps:

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake .. # For ARM Build
$ make
```
