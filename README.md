# sitec-cpp 

Supporting library for sitec systems Linux based systems. This library contains
the following subdirectories:

  * `can`

## Build dependencies

  * C++ 14 compliant compiler
  * `libsocketcan`

## Build

If you build the project and you want to use machine depnendent features (like `can::InterfaceConfiguration`)
you need to configure the machine which are you building for.

Default the library will be built for the S4 Comm Pro X device. If you want to build for the S7 Track Pro X
set the following option during configuration with cmake:

```
$ mkdir build
$ cd build/
$ cmake -D BUILD_S7:ON ../
```

The libraries will build as static libraries by default. If you need shared libraries use the following configuration
option during configuration step:

```
$ mkdir build
$ cd build/
$ cmake -D BUILD_SHARED:ON ../
```
