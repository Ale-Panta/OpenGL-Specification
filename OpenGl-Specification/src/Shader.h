#pragma once
#include "GL/glew.h"

#include <string>

namespace OpenGL
{
	/**
	 * To use the shader program call glUseProgram(), whitch loads the program containing the two compiled
	 * shaders into the OpenGL pipeline stages (onto the GPU!).
	 * #NOTE: glUseProgram doesn't run the shaders. It just loads them onto the hardware.
	 */
	class Shader
	{
	public:
		Shader() = default;
		Shader(const char* vertShaderSrc, const char* fragShaderSrc);
		~Shader();

	public:
		void UploadUniform1f(const char* uniformName, float value);

		operator GLuint() const { return m_ShaderProgram; }

	private:
		/**
		 * Read the files given as input and return the content.
		 */
		std::string ReadShaderSrc(const char* filePath);

		// Catch GLSL errors
		void PrintShaderLog(GLuint shader);
		void PrintProgramLog(GLuint program);
		bool CheckOpenGLError();

	private:
		GLuint m_ShaderProgram = 0;
	};
}