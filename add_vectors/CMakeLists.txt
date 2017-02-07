cmake_minimum_required (VERSION 2.8.11)
project (opencl_vector_add)

find_package (OpenCL)
if( NOT OpenCL_FOUND )
	MESSAGE( SEND_ERROR "OpenCL not found. Install SDK from NVidia (CUDA), AMD or Intell" )
endif( NOT OpenCL_FOUND )

include_directories ("${PROJECT_SOURCE_DIR}/../include" ${OpenCL_INCLUDE_DIRS})

add_executable(vector_add vector_add.cpp)
target_link_libraries(vector_add ${OpenCL_LIBRARIES})

