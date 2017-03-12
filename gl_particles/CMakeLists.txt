include_directories ("${PROJECT_SOURCE_DIR}/include" ${OpenCL_INCLUDE_DIR} 
					${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIR} 
                    ${GLEW_INCLUDE_DIR}  ${GLM_INCLUDE_DIR})

add_executable(ocl_gl_particles OpenCLParticles.cpp GLSLProgram.cpp GLSLProgram.h GlutWindow.cpp GlutWindow.h VertexArray.h VertexBuffer.cpp VertexBuffer.h)
target_compile_features(ocl_gl_particles PRIVATE cxx_range_for)
target_link_libraries(ocl_gl_particles ${OpenCL_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})

