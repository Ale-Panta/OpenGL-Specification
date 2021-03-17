#pragma once

#include "Shader.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLM/ext/matrix_float4x2_precision.hpp>

namespace OpenGL
{
	/**
	 * Vertex contains useful values such as:
	 * position, texture coordinates, normal, tangent, color.
	 */
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		glm::vec3 STangent;
		glm::vec3 TTangent;
	};

	struct Texture
	{
		unsigned int ID;
		std::string Type;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

	public:
		/**
		 * Activate the proper VAO and textures slot.
		 * Call glDrawElements().
		 */
		void Draw(Shader& shader);

	protected:
		/**
		 * Setup vertex and indices buffers, attributes, textures.
		 */
		void SetupMesh();

	public:
		// Mesh data
		std::vector<Vertex> Vertices;
		std::vector<Texture> Textures;
		std::vector<GLuint> VertexIndices;
		std::vector<GLuint> TexCoordIndices;
		std::vector<GLuint> NormalsIndices;
		Texture TextureOne;

	private:
		/**
		 * Vertex array object
		 * #TODO: Explain what is it
		 */
		GLuint VAO = 0;

		/**
		 * Vertex buffer object
		 * #TODO: Explain what is it
		 */
		GLuint VBO = 0;

		/**
		 * Element buffer object
		 * #TODO: Explain what is it.
		 */
		GLuint EBO = 0;
	};
}