#pragma once

#include "Material.h"
#include "Texture.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLM/ext/matrix_float4x2_precision.hpp>
#include <glm/ext/matrix_transform.hpp>

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
		glm::vec3 Tangent;
		glm::vec3 Color;
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
		virtual void Draw(Material& material);
		void SetPosition(const glm::vec3& RefPosition) { m_Position = RefPosition; }
		const glm::mat4& GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), m_Position); }

	protected:
		/**
		 * Setup vertex and indices buffers, attributes, textures.
		 */
		virtual void SetupMesh();

	public:
		glm::vec3 m_Position = glm::vec3(0.0f);

		// Mesh data
		std::vector<Vertex> Vertices;
		std::vector<Texture*> Textures;
		std::vector<GLuint> VertexIndices;
		std::vector<GLuint> TexCoordIndices;
		std::vector<GLuint> NormalsIndices;

	protected:
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