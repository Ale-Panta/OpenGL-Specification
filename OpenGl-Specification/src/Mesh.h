#pragma once

#include "Shader.h"

#include <vector>
#include <GLM/ext/matrix_float4x2_precision.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "GL/glew.h"

namespace OpenGL
{
	/**
	 * Vertex data structure.
	 */
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Color;
	};

	/**
	 * Mesh contains data to vertex position. This allow us to store in one point the geometry we want to draw.
	 * @see Vertex.
	 */
	class Mesh
	{
	public:
		Mesh() = default;

		/**
		 * Mesh constructor.
		 * @param vertices - list / vector of Vertex.
		 * @param indices - list / vector of GLuint.
		 * @see Vertex.
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

	public:
		/**
		 * Draw the geometry with the program passed as parameter.
		 * Enable depth test and cull face. Check if this settings fit for you.
		 * Bind the proper VAO and call glDrawElements.
		 * @param shader - shader program.
		 * @see Shader.
		 */
		virtual void Draw(Shader& shader);

	protected:
		/** Setup vertex and indices buffers, attributes. */
		virtual void SetupMesh();

	public:
		/** Collection of all vertex data. */
		std::vector<Vertex> Vertices;

		/** Collection of all vertex indices. */
		std::vector<GLuint> VertexIndices;

	protected:
		/**
		 * Vertex array object.
		 */
		GLuint VAO = 0;

		/**
		 * Vertex buffer object.
		 */
		GLuint VBO = 0;

		/**
		 * Element buffer object.
		 */
		GLuint EBO = 0;
	};
}