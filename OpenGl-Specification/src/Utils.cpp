#include "Utils.h"

#include <fstream>
#include <iostream>

using namespace std;

namespace OpenGl
{
	GLuint Utils::CreateShaderProgram(const char* vp, const char* fp)
	{
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;

		string vertShaderStr = ReadShaderSource(vp);
		string fragShaderStr = ReadShaderSource(fp);

		const char* vertShaderSrc = vertShaderStr.c_str();
		const char* fragShaderSrc = fragShaderStr.c_str();

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShader, 1, &vertShaderSrc, NULL);
		glShaderSource(fShader, 1, &fragShaderSrc, NULL);

		glCompileShader(vShader);
		CheckOpenGLError();
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1)
		{
			cout << "vertex compilation failed" << endl;
			PrintShaderLog(vShader);
		}

		glCompileShader(fShader);
		CheckOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1)
		{
			cout << "fragment compilation failed" << endl;
			PrintShaderLog(fShader);
		}

		GLuint vfProgram = glCreateProgram();
		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);

		glLinkProgram(vfProgram);
		CheckOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1)
		{
			cout << "linking failed" << endl;
			PrintProgramLog(vfProgram);
		}

		return vfProgram;
	}

	GLuint Utils::CreateShaderProgram(const char* vp, const char* gp, const char* fp)
	{
		return 0;
	}

	GLuint Utils::CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp)
	{

		return 0;
	}

	GLuint Utils::CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp)
	{

		return 0;
	}

	void Utils::PrintShaderLog(GLuint shader)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		if (len > 0)
		{
			log = (char*)malloc(len);

			glGetShaderInfoLog(shader, len, &chWrittn, log);
			cout << "Shader Info Log:" << log << endl;
			free(log);
		}
	}

	void Utils::PrintProgramLog(GLuint program)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		if (len > 0)
		{
			log = (char*)malloc(len);

			glGetShaderInfoLog(program, len, &chWrittn, log);
			cout << "Shader Info Log:" << log << endl;
			free(log);
		}
	}

	bool Utils::CheckOpenGLError()
	{
		bool isAnyErrorFound = false;
		int glError = glGetError();
		while (glError != GL_NO_ERROR)
		{
			cout << "glError" << glError << endl;
			isAnyErrorFound = true;
			glError = glGetError();
		}

		return isAnyErrorFound;
	}

	string Utils::ReadShaderSource(const char* filePath)
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
}