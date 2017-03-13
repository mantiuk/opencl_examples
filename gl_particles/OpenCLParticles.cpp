/*
 * OpenCL + OpenGL particle animation example.
 *
 * The particles are attracted to the mouse pointer. Move it around to create interesting animation pattarens. 
 *
 * The code is based on the example by Alexey V. Boreskov. A few classes are from the original example. However, 
 * the code has been modernized and simplified:
 *  - More interesting particle animation
 *  - The original code was a monolitic tangle of C++ classes and 20+ files. Only relevant classes were left.
 *  - OpenCL calls use CPP interface instead of C99 API.
 *  - glm instead of a custom math classes.
 *  - Exceptions instead of if's
 *
 * (c) 2017 Rafal K. Mantiuk
*/

#include <gl/glew.h>

#if __APPLE__
  #include <OpenCL/opencl.h>
  #include <OpenCL/cl_gl.h>
#else
  #include <CL/opencl.h>
  #include <CL/cl_gl.h>
#endif

#include "GlutWindow.h"
#include "GLSLProgram.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#define	NUM_PARTICLES	(2048)

class	ParticlesWindow : public GlutWindow
{
	glm::vec3         p[NUM_PARTICLES];
	glm::vec3         v[NUM_PARTICLES];
	glm::vec3         force[NUM_PARTICLES];
	glm::vec3         color[NUM_PARTICLES];
	GLSLProgram      program;
	VertexArray  vao;
	VertexBuffer posBuf, velBuf, forceBuf, colorBuf;

	SourceFile opencl_source;

	// OpenCL part
	cl::Context context;
	cl::Program cl_program;
	cl::Kernel kernel;
	cl::CommandQueue queue;

	cl::BufferGL cl_pos_buf;
	cl::BufferGL cl_vel_buf;
	cl::BufferGL cl_force_buf;

	float mouse_x = 0.f, mouse_y = 0.f;

public:
	ParticlesWindow() : GlutWindow(30, 30, 1200, 1000, "OpenCL particles")
	{
		try {

			program.loadProgram("particles");

			initParticles();

			posBuf.create();
			posBuf.bind(GL_ARRAY_BUFFER);
			posBuf.setData(NUM_PARTICLES * sizeof(glm::vec3), p, GL_DYNAMIC_DRAW);
			posBuf.unbind();
			velBuf.create();
			velBuf.bind(GL_ARRAY_BUFFER);
			velBuf.setData(NUM_PARTICLES * sizeof(glm::vec3), v, GL_DYNAMIC_DRAW);
			velBuf.unbind();
			forceBuf.create();
			forceBuf.bind(GL_ARRAY_BUFFER);
			forceBuf.setData(NUM_PARTICLES * sizeof(glm::vec3), force, GL_DYNAMIC_DRAW);
			forceBuf.unbind();
			colorBuf.create();
			colorBuf.bind(GL_ARRAY_BUFFER);
			colorBuf.setData(NUM_PARTICLES * sizeof(glm::vec3), color, GL_STATIC_DRAW);
			colorBuf.unbind();

			initOpenCL();

			cl_pos_buf = cl::BufferGL(context, CL_MEM_READ_WRITE, posBuf.getId());
			cl_vel_buf = cl::BufferGL(context, CL_MEM_READ_WRITE, velBuf.getId());
			cl_force_buf = cl::BufferGL(context, CL_MEM_READ_WRITE, forceBuf.getId());

			vao.create();
			vao.bind();
			posBuf.bind(GL_ARRAY_BUFFER);
			posBuf.setAttrPtr(0, 3, sizeof(glm::vec3), (void *)0);
			colorBuf.bind(GL_ARRAY_BUFFER);
			colorBuf.setAttrPtr(3, 3, sizeof(glm::vec3), (void *)0);
			vao.unbind();
		}
		catch (cl::Error err)
		{
			printf("Error: %s (%d)\n", err.what(), err.err());
			exit(1);
		}
		catch (GLSLProgramException err)
		{
			printf("GLSL error: %s\n", err.what() );
			exit(1);
		}
	}

	void	initOpenCL()
	{

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		if (platforms.size() == 0) {
			throw cl::Error(-1, "No OpenCL platform found");
		}

		int use_platform = 0;

		std::string platform_name;
		platforms[use_platform].getInfo(CL_PLATFORM_NAME, &platform_name);
		printf("Using platform: %s\n", platform_name.c_str());


#ifdef	_WIN32
		cl_context_properties properties[] =
		{
			CL_GL_CONTEXT_KHR,
			(cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR,
			(cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)(platforms[use_platform])(),
			0      // terminator
		};
#elif __APPLE__
		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)(platforms[use_platform])(),
			0      // terminator
		};
		//	#error "TODO: setup shared OpenGL / OpenCL rendering context on Mac"

#else
		cl_context_properties properties[] =
		{
			CL_GL_CONTEXT_KHR,
			(cl_context_properties)glXGetCurrentContext(),
			CL_GLX_DISPLAY_KHR,
			(cl_context_properties)glXGetCurrentDisplay(),
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)(platforms[use_platform])(),
			0      // terminator
		};
#endif

		context = cl::Context(CL_DEVICE_TYPE_GPU, properties);

		std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

		opencl_source.load("particles.cl");
		const char *source = opencl_source.c_str();

		cl::Program::Sources cl_source(1,
			std::make_pair(opencl_source.c_str(), opencl_source.length()));
		cl_program = cl::Program(context, cl_source);

		try {
			cl_program.build(devices);
		}
		catch (cl::Error err)
		{
			printf("Compilation failed:\n");
			printf("Log: %s\n", cl_program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]).c_str());
			throw err;
		}

		kernel = cl::Kernel(cl_program, "animate");

		queue = cl::CommandQueue(context, devices[0], 0);
	}

	void	doOpenCL(float dt)
	{
		int    n = NUM_PARTICLES;
		size_t localSize = 512;			// work-items per work-group
		size_t globalSize = n;				// total work-items

		glFinish();

		std::vector<cl::Memory> cl_vbos;
		cl_vbos.push_back(cl_pos_buf);
		cl_vbos.push_back(cl_vel_buf);
		cl_vbos.push_back(cl_force_buf);

		queue.enqueueAcquireGLObjects(&cl_vbos);

		kernel.setArg(0, cl_pos_buf);
		kernel.setArg(1, cl_vel_buf);
		kernel.setArg(2, cl_vel_buf);

		kernel.setArg(3, mouse_x);
		kernel.setArg(4, mouse_y);

		kernel.setArg(5, dt);
		kernel.setArg(6, n);

		queue.enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(globalSize),
			cl::NDRange(localSize),
			NULL,
			NULL);

		queue.enqueueReleaseGLObjects(&cl_vbos);

		queue.finish();
	}

	void	initParticles()
	{
		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			p[i] = glm::vec3(glm::linearRand(-0.6, 0.6), glm::linearRand(-0.6, 0.6), 0);
			v[i] = glm::vec3(glm::linearRand(-0.1, 0.1), glm::linearRand(-0.1, 0.1), 0);
			force[i] = glm::vec3(glm::linearRand(-1, 1), glm::linearRand(-1, 1), 0);
			color[i] = glm::vec3(glm::linearRand(0.2f, 1.f), glm::linearRand(0.2f, 1.f), glm::linearRand(0.2f, 1.f));
		}
	}

	void redisplay()
	{
		static float lastTime = 0;

		float t = 0.001f * glutGet(GLUT_ELAPSED_TIME);

		doOpenCL(lastTime - t);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 mv(1.0); // = getRotation();

		program.bind();
		program.setUniform("mv", mv);
		vao.bind();

		glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

		vao.unbind();
		program.unbind();

		lastTime = t;
	}

	void reshape(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	}

	virtual void mousePassiveMotion(int x, int y) {
		mouse_x = ((float)x / (float)getWidth()) * 2. - 1.;
		mouse_y = 1. - ((float)y / (float)getHeight()) * 2.;

		//printf("x = %g, y = %g\n", mouse_x, mouse_y );
	}
};

int main(int argc, char * argv[])
{
	GlutWindow::init(argc, argv);

	ParticlesWindow	win;

	GlutWindow::run();

	return 0;
}

