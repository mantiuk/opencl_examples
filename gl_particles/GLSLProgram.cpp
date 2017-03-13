#include "GLSLProgram.h"

#include <sys/stat.h>
#include <glm/gtc/type_ptr.hpp>


GLSLProgram::GLSLProgram() : program( 0 ), compile_log( NULL )
{
	for (int kk = 0; kk < SHADER_TYPE_COUNT; kk++)
		shaders[kk] = 0;
	
}


static const char * getGlErrorString()
{
	int error;

	if ((error = glGetError()) != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_NO_ERROR:				// no error, return NULL
			return NULL;

		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";

		case GL_INVALID_VALUE:
			return "GL_INVALID_VALUE";

		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";

		case GL_OUT_OF_MEMORY:
			return  "GL_OUT_OF_MEMORY";

		default:
			return "UNKNOWN ERROR";
		}
	}

	return NULL;
}


void GLSLProgram::checkGlError(const char * title)
{
	const char * str = getGlErrorString();

	if (str != NULL) {
		std::string msg("GLSLProgram Error");
		if (title != NULL) {
			msg += "(";
			msg += title;
			msg += ")";
		}
		msg += str;
		throw GLSLProgramException(msg.c_str());
	}
}


void GLSLProgram::loadProgram(const std::string& fileName)
{
	if (program == 0)
		program = glCreateProgram();

	// check for errors
	checkGlError();

	struct ShaderType
	{
		const char * name;
		GLenum		 type;
	};

	ShaderType info[] =
	{
		{ "vertex", GL_VERTEX_SHADER },
		{ "fragment", GL_FRAGMENT_SHADER },
		{ "geometry", GL_GEOMETRY_SHADER },
		{ "tesscontrol", GL_TESS_CONTROL_SHADER },
		{ "tesseval", GL_TESS_EVALUATION_SHADER }
	};

	SourceFile source;
	for (int kk = 0; kk < SHADER_TYPE_COUNT; kk++) {
		std::string full_name = std::string(fileName) + "_" + info[kk].name + ".glsl";

		// Check if file exists
		struct stat buffer;
		if (stat(full_name.c_str(), &buffer) == 0) {
			source.load(full_name.c_str());

			GLuint	shader = glCreateShader(info[kk].type);

			const char* srcs[] = { source.c_str() };
			int lens[] = { (int)source.length() };
			glShaderSource(shader, 1, srcs, lens);
			glCompileShader(shader);

			GLint        compileStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

			if (!compileStatus) {
				std::string msg("Error: Failed to compile GLSL shader: ");
				msg += getShaderLog(shader);
				throw GLSLProgramException( msg.c_str() );
			}

			glAttachShader(program, shader);
			shaders[kk] = shader;

		}

	}
	glLinkProgram(program);
	checkGlError();
}


const char *GLSLProgram::getShaderLog(GLuint object)
{
	GLint       logLength = 0;
	GLsizei     charsWritten = 0;

	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);

	checkGlError();

	if ( logLength < 1)          // check for OpenGL errors
		throw GLSLProgramException("Error: Cannot get shader compiler log");

	delete[] compile_log;

	compile_log = new char[logLength];

	glGetShaderInfoLog(object, logLength, &charsWritten, (GLcharARB*)compile_log);

	return compile_log;
}

void GLSLProgram::bind()
{
	glUseProgram(program);
}

void GLSLProgram::unbind()
{
	glUseProgram(0);
}

void GLSLProgram::setUniform(const char * name, const glm::mat4 &mat)
{
	int loc = glGetUniformLocation(program, name);
	if (loc < 0)
		throw GLSLProgramException("Failed to find uniform");

	glUniformMatrix4fv(loc, 1, GL_TRUE, glm::value_ptr(mat));		// require matrix transpose
}


GLSLProgram::~GLSLProgram()
{
	delete[] compile_log;
	glDeleteProgram(program);                  // it will also detach shaders
	for (int kk = 0; kk < SHADER_TYPE_COUNT; kk++) {
		glDeleteShader(shaders[kk]);
	}
}

