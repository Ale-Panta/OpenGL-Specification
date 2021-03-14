#pragma once
#include "GL/glew.h"

#include <string>
#include <glm/glm.hpp>

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
		operator GLuint() const { return m_ShaderProgram; }	// Overload operator.

		void SetUniformBool(const char* uniformName, bool value);
		void SetUniformInt(const char* uniformName, int value);
		void SetUniformFloat(const char* uniformName, float value);
		void SetUniformVec3(const char* uniformName, const glm::vec3& value);
		void SetUniformMatrix4(const char* uniformName, const glm::mat4& matrix, GLboolean transpose = GL_FALSE);

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