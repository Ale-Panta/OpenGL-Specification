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

		/**
		 * Shader constructor.
		 * @param vertShaderSrc - vertex shader file path.
		 * @param fragShaderSrc - fragment shader file path.
		 */
		Shader(const char* vertShaderSrc, const char* fragShaderSrc);
		~Shader();

		/**
		 * Set uniform bool value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - bool value to assign to shader program.
		 */
		void SetUniformBool(const char* uniformName, bool value);

		/**
		 * Set uniform int value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - int value to assign to shader program.
		 */
		void SetUniformInt(const char* uniformName, int value);

		/**
		 * Set uniform uint value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - int value to assign to shader program.
		 */
		void SetUniformUInt(const char* uniformName, unsigned int value);

		/**
		 * Set uniform float value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - float value to assign to shader program.
		 */
		void SetUniformFloat(const char* uniformName, float value);

		/**
		 * Set uniform vec3 value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - vec3 value to assign to shader program.
		 */
		void SetUniformVec3(const char* uniformName, const glm::vec3& value);

		/**
		 * Set uniform vec4 value in shader program.
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param value - vec4 value to assign to shader program.
		 */
		void SetUniformVec4(const char* uniformName, const glm::vec4& value);

		/**
		 * Set uniform mat4 value in shader program.
		 *
		 * @param uniformName - name of the uniform inside the shader program.
		 * @param matrix - mat4 value to assign to shader program.
		 * @param transpose - pass Glboolean = true to assign the transpose.
		 */
		void SetUniformMatrix4(const char* uniformName, const glm::mat4& matrix, GLboolean transpose = GL_FALSE);

		operator GLuint() const { return m_ShaderProgram; }

	private:
		/**
		 * Read the files given as input and return the content.
		 * @param filePath, the shader file to parse.
		 */
		std::string ReadShaderSrc(const char* filePath);

		/**
		 * Print shader compilation error.
		 * @param shader - shader get from glGetShaderiv.
		 */
		void PrintShaderLog(GLuint shader);

		/**
		 * Print link compilation error.
		 * @param program, shader get from glGetProgramiv.
		 */
		void PrintProgramLog(GLuint program);

		/**
		 * Check if there is any error during flow.
		 * @return false if any error occurs.
		 */
		bool CheckOpenGLError();

	private:
		GLuint m_ShaderProgram = 0;
	};
}