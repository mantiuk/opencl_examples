## Synopsis

This is a set of examples showing how to write parallelized code using OpenCL. All examples are in C++ and use the official OpenCL C++ wrapper (cl.hpp), which makes the code more concise and readable. Examples use OpenCL 1.2 for better portability.

## Examples

* add_vectors - Starting point for learning OpenCL; just adding two vectors

* list_platforms - List available OpenCL platforms and some information on OpenCL capabilities

* gl_particles - A simple particle simulation and an example of interoperability between OpenCL and OpenGL

## Motivation

There is a good number of OpenCL tutorials and examples available on-line but most of them use a spartan C99 OpenCL API. OpenCL is much easier to learn and program using the official C++ wrapper. C++ wrapper makes the code much more concise and protects against making some common mistakes. 

There are also very few examples showing how OpenGL can be integrated with OpenGL. "gl_particles" serves this purpose. 

## Installation

You need to install OpenCL SDK for your platform. Only xcode is required on Mac. 

* For Nvidia, OpenCL SDK is confusingly bundled with CUDA Toolkit
https://developer.nvidia.com/cuda-downloads 

On Ubuntu (tested on 18.04/bionic) it is possible to install the required files from packages:

sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt-get update
sudo apt-get install nvidia-cuda-toolkit

* Intel: 
https://software.intel.com/en-us/intel-opencl

* AMD:
Linux: OpenCL ICD is bundled with AMD drivers. Get your card's driver from here: https://www.amd.com/en/support . The required headers can be installed using your package manager: sudo apt-get install opencl-headers

Windows: AMD has dropped the support for APP SDK and such any related package might not available on developer.amd.com. You can instead use this light SDK package available here: https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases


Use CMake to generate build files for your platform (VS, gnu, xcode, etc.)
https://cmake.org/


"gl_particles" example requires a few more dependencies. If these are not found, the OpenGL example will be skipped from build but the rest of the examples will compile. The required libraries are:

* FreeGLUT: http://freeglut.sourceforge.net/

* GLEW: http://glew.sourceforge.net/

* GLM: http://glm.g-truc.net/

On Linux the libraries can be easily installed from packages. Remember to install the development version of the packages (freeglut3-dev, libglew-dev, libglm-dev). 

On Windows the libraries needs to be manually installed and then the path specified for each library in CMake. 

The OpenGL example currently does not work on OSX.

## Running

Note that all examples must be run from the corresponding source directory. This is because each example needs to load OpenCL kernels (and OpenGL shaders) from the text file, which can be found in the source directories. 




