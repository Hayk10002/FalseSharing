# FalseSharing

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)
- [Possible Output](#possible-output)
- [Understanding False Sharing](#understanding-false-sharing)

## Introduction
The purpose of this project is to demonstrate the concept of false sharing in multithreaded applications and its impact on performance.

## Build and Run
To clone and run this project, you'll need [Git](https://git-scm.com) and [CMake](https://cmake.org/) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/Hayk10002/FalseSharing

# Go into the repository
$ cd FalseSharing

# Generate the build files
$ cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

# Build the project
$ cmake --build build --config Release
```

Then, run the executable generated in the `build` directory.

## Possible Output

```
Time when false sharing:     1297ms
Time when not false sharing: 287ms
Time non-threaded:           540ms
```

## Understanding False Sharing
False sharing occurs when threads on different processors modify variables that reside on the same cache line. This can lead to significant performance degradation due to the overhead of maintaining cache coherence. In this project, we illustrate how false sharing can impact the performance of a multithreaded application and discuss strategies to mitigate it, such as padding data structures to ensure that frequently modified variables do not share cache lines.
