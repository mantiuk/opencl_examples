cmake_minimum_required (VERSION 2.8.11)
project (list_platforms)

find_package (OpenCL)
if( NOT OpenCL_FOUND )
	MESSAGE( SEND_ERROR "OpenCL not found. Install SDK from NVidia (CUDA), AMD or Intell" )
endif( NOT OpenCL_FOUND )

include_directories ("${PROJECT_SOURCE_DIR}/../include" ${OpenCL_INCLUDE_DIRS})

add_subdirectory (vector_add) 
add_subdirectory (list_platforms) 

