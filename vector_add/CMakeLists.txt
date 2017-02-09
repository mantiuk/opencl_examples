include_directories ("${PROJECT_SOURCE_DIR}/../include" ${OpenCL_INCLUDE_DIRS})

add_executable(ocl_vector_add vector_add.cpp)
target_compile_features(ocl_vector_add PRIVATE cxx_range_for)
target_link_libraries(ocl_vector_add ${OpenCL_LIBRARIES})

