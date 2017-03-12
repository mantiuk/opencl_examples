cmake_minimum_required (VERSION 2.8.11)
project (opencl_examples)

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

find_package (OpenCL)
if( NOT OpenCL_FOUND )
	MESSAGE( SEND_ERROR "OpenCL not found. Install SDK from NVidia (CUDA), AMD or Intell" )
endif( NOT OpenCL_FOUND )

find_package (OpenGL)
if( NOT OpenGL_FOUND )
	MESSAGE( WARNING "OpenGL development files not found." )
endif( NOT OpenGL_FOUND )


find_package (GLUT)
if( NOT GLUT_FOUND )
	MESSAGE( WARNING "GLUT not found. Download and install FreeGLUT: http://freeglut.sourceforge.net/" )
endif( NOT GLUT_FOUND )

find_package (GLEW)
if( NOT GLEW_FOUND )
	MESSAGE( WARNING "GLEW not found. Download and install GLEW from: http://glew.sourceforge.net/" )
endif( NOT GLEW_FOUND )

find_package (GLM)
if( NOT GLM_FOUND )
	MESSAGE( WARNING "GLM not found. Download and install GLM from: http://glm.g-truc.net/" )
endif( NOT GLM_FOUND )

include_directories ("${PROJECT_SOURCE_DIR}/../include" ${OpenCL_INCLUDE_DIRS})

add_subdirectory (vector_add) 
add_subdirectory (list_platforms) 

if( OPENGL_FOUND AND GLUT_FOUND AND GLEW_FOUND AND GLM_FOUND )
	add_subdirectory (gl_particles) 
else( OPENGL_FOUND AND GLUT_FOUND AND GLEW_FOUND AND GLM_FOUND )
	MESSAGE( STATUS "One of the dependecies for OpenGL examples not found. Those examples will not be compiled. Please check the log above." )
endif( OPENGL_FOUND AND GLUT_FOUND AND GLEW_FOUND AND GLM_FOUND )

