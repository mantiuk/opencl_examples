include_directories ("${PROJECT_SOURCE_DIR}/include" ${OpenCL_INCLUDE_DIRS})

add_executable(ocl_list_platforms list_platforms.cpp)
target_compile_features(ocl_list_platforms PRIVATE cxx_range_for)
target_link_libraries(ocl_list_platforms ${OpenCL_LIBRARIES})

