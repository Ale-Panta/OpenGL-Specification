#pragma once
#include <GL/glew.h>
#include <string>

namespace OpenGl
{
	class Utils
	{
	public:
		static GLuint CreateShaderProgram(const char* vp, const char* fp);
		static GLuint CreateShaderProgram(const char* vp, const char* gp, const char* fp);
		static GLuint CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp);
		static GLuint CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp);

	private:
		static void PrintShaderLog(GLuint shader);
		static void PrintProgramLog(GLuint program);
		static bool CheckOpenGLError();
		static std::string ReadShaderSource(const char* filePath);
	};
}