## Synopsis

This is a set of examples showing how to write parallelized code using OpenCL. All examples are in C++ and use the official OpenCL C++ wrapper (cl.hpp), which makes the code more concise and readable. Examples use OpenCL 1.2 for better portability.

## Examples

add_vectors - Starting point for learning OpenCL; just adding two vectors

## Motivation

TODO

## Installation

You need to install OpenCL SDK for your platform. Only xcode is required on Mac. 

For Nvidia, OpenCL SDK is confusingly bundled with CUDA Toolkit
https://developer.nvidia.com/cuda-downloads 

Intel: 
https://software.intel.com/en-us/intel-opencl

AMD:
http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/

Use CMake to generate build files for your platform (VS, gnu, xcode, etc.)
https://cmake.org/

