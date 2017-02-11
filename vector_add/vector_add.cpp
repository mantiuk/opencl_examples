#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <exception>

#include <math.h>

#define __CL_ENABLE_EXCEPTIONS

#if __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/opencl.h>
#endif

#include <CL/cl.hpp>
#include <iostream>

int main(){
	try{

		//get all platforms (drivers)
		std::vector<cl::Platform> all_platforms;
		cl::Platform::get(&all_platforms);
		if (all_platforms.size() == 0){
			std::cout << " No platforms found. Check OpenCL installation!\n";
			exit(1);
		}
		cl::Platform default_platform = all_platforms[0];
		std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";

		//get default device of the default platform
		std::vector<cl::Device> all_devices;
		default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
		if (all_devices.size() == 0){
			std::cout << " No devices found. Check OpenCL installation!\n";
			exit(1);
		}
		cl::Device default_device = all_devices[0];
		std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

		cl::Context context({ default_device });

		cl::Program::Sources sources;
		// kernel calculates for each element C=A+B
		std::string kernel_code =
			"__kernel void simple_add(__global const int* A, __global const int* B, __global int* C) {"
			"	int index = get_global_id(0);"
			"	C[index] = A[index] + B[index];"
			"};";
		sources.push_back({ kernel_code.c_str(), kernel_code.length() });

		cl::Program program(context, sources);
		try {
			program.build({ default_device });
		}
		catch (cl::Error err) {
			std::cout << " Error building: " << 
				program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
			exit(1);
		}

		// create buffers on the device
		cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
		cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
		cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);

		int A[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };

		//create queue to which we will push commands for 	the device.
		cl::CommandQueue queue(context, default_device);

		//write arrays A and B to the device
		queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
		queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);

		cl::Kernel kernel(program, "simple_add");

		kernel.setArg(0, buffer_A);
		kernel.setArg(1, buffer_B);
		kernel.setArg(2, buffer_C);
		queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(10), cl::NullRange);

		int C[10];
		//read result C from the device to array C
		queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, C);
		queue.finish();

		std::cout << " result: \n";
		for (int i = 0; i < 10; i++){
			std::cout << C[i] << " ";
		}
		std::cout << std::endl;
	}
	catch (cl::Error err)
	{
		printf("Error: %s (%d)\n", err.what(), err.err());
		return -1;
	}

    return 0;
}	
