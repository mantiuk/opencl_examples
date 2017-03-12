#pragma once
#ifndef __GLSLPROGRAM__
#define __GLSLPROGRAM__

#include <string>
#include <fstream>
#include <exception>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>


class GLSLProgramException : public std::exception
{
	std::string msg;
public:
	GLSLProgramException(const char *message) : msg(message)
	{
	}

	const char* what()
	{
		return msg.c_str();
	}

};

typedef std::shared_ptr<char*> GLSLShadferLog;

class SourceFile
{
	char* source;
	size_t size;
public:
	SourceFile() : source(NULL)
	{
	}

	~SourceFile()
	{
		delete[] source;
	}

	const char *c_str()
	{
		return source;
	}

	size_t length()
	{
		return size;
	}

	void load(const char *fname)
	{
		std::ifstream file(fname, std::ios::binary | std::ios::ate);
		size = (size_t)file.tellg();
		file.seekg(0, std::ios::beg);

		source = new char[size + 1];
		if (!file.read(source, size))
			throw GLSLProgramException("Error: Failed load kernel source from file");

		source[size] = 0;
	}

};

#define SHADER_TYPE_COUNT 5

class GLSLProgram
{
	GLuint 	program;                            // program object handle
	GLuint 	shaders[SHADER_TYPE_COUNT];
	const char *compile_log;

	void checkGlError(const char * title = NULL);

	const char *getShaderLog(GLuint object);

public:
	GLSLProgram();

	void loadProgram(const std::string& fileName);

	void bind();
	void unbind();

	void setUniform(const char * name, const glm::mat4 &mat);

	~GLSLProgram();
};

#endif
