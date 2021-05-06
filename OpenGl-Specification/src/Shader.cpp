#include "Shader.h"

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

namespace OpenGL
{
	Shader::Shader(const char* vertShaderSrc, const char* fragShaderSrc)
	{
		// Generate shader of type vertex and fragment.
		// OpenGL create each shader object (initially empty), and returns an integer ID.
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		string vertShaderStr = ReadShaderSrc(vertShaderSrc);
		string fragShaderStr = ReadShaderSrc(fragShaderSrc);

		const char* vertShaderCode = vertShaderStr.c_str();
		const char* fragShaderCode = fragShaderStr.c_str();

		// Loads the GLSL code from the strings into the empty shader objects.
		glShaderSource(vertShader, 1, &vertShaderCode, nullptr);
		glShaderSource(fragShader, 1, &fragShaderCode, nullptr);

		// Handle compiling errors.
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;

		// The shaders are them each compiled as follow...
		glCompileShader(vertShader);

		#pragma region Handle error
		// Checking errors.
		CheckOpenGLError(); 
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) 
		{ 
			cout << "vertex compilation failed" << endl; 
			PrintShaderLog(vertShader);
		}
#pragma endregion

		glCompileShader(fragShader);

		#pragma region Handle error
		// Checking errors.
		CheckOpenGLError();
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1)
		{
			cout << "vertex compilation failed" << endl;
			PrintShaderLog(fragShader);
		}
#pragma endregion

		// Create a program that contains a series of compiled shaders.
		m_ShaderProgram = glCreateProgram();
		// Attach each shader to the program.
		glAttachShader(m_ShaderProgram, vertShader);
		glAttachShader(m_ShaderProgram, fragShader);

		// Request that the GLSL compiler ensure that they are compatible.
		glLinkProgram(m_ShaderProgram);

		#pragma region Handle error
		// Checking errors.
		CheckOpenGLError(); 
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &linked); 
		if (linked != 1) 
		{ 
			cout << "linking failed" << endl; 
			PrintProgramLog(m_ShaderProgram); 
		}
#pragma endregion

		// Delete the shaders once they are linked, we no longer need them.
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderProgram);
		std::cout << "Delete shader program: {" << m_ShaderProgram << "}" << std::endl;
	}

	void Shader::SetUniformBool(const char* uniformName, bool value)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniform1i(m_ShaderProgram, uLocation, (int)value);
	}

	void Shader::SetUniformInt(const char* uniformName, int value)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniform1i(m_ShaderProgram, uLocation, value);
	}

	void Shader::SetUniformFloat(const char* uniformName, float value)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniform1f(m_ShaderProgram, uLocation, value);
	}

	void Shader::SetUniformVec3(const char* uniformName, const glm::vec3& value)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniform3fv(m_ShaderProgram, uLocation, 1, glm::value_ptr(value));
	}

	void Shader::SetUniformVec4(const char* uniformName, const glm::vec4& value)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniform4fv(m_ShaderProgram, uLocation, 1, glm::value_ptr(value));
	}

	void Shader::SetUniformMatrix4(const char* uniformName, const glm::mat4& matrix, GLboolean transpose)
	{
		GLuint uLocation = glGetUniformLocation(m_ShaderProgram, uniformName);
		glProgramUniformMatrix4fv(m_ShaderProgram, uLocation, 1, transpose, glm::value_ptr(matrix));
	}

	std::string Shader::ReadShaderSrc(const char* filePath)
	{
		string content;
		ifstream fileStream(filePath, ios::in);
		string line = "";

		while (!fileStream.eof())
		{
			getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}

	void Shader::PrintShaderLog(GLuint shader)
	{
		int len = 0; 
		int chWrittn = 0; 
		char* log; 
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		if (len > 0) 
		{
			log = (char*)malloc(len); 
			glGetShaderInfoLog(shader, len, &chWrittn, log); 
			cout << "Shader Info Log: " << log << endl; free(log); 
		}
	}

	void Shader::PrintProgramLog(GLuint program)
	{
		int len = 0; 
		int chWrittn = 0; 
		char* log;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		if (len > 0) 
		{ 
			log = (char*)malloc(len); 
			glGetProgramInfoLog(program, len, &chWrittn, log);
			cout << "Program Info Log: " << log << endl; free(log); 
		}
	}

	bool Shader::CheckOpenGLError()
	{
		bool foundError = false; 
		int glErr = glGetError(); 

		while (glErr != GL_NO_ERROR) 
		{ 
			cout << "glError: " << glErr << endl; 
			foundError = true; 
			glErr = glGetError(); 
		}

		return foundError;
	}

}