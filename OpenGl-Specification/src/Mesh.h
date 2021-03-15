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
		void Draw(Shader& shader);

	protected:
		void SetupMesh();

	public:
		// Mesh data
		std::vector<Vertex> Vertices;
		std::vector<GLuint> Indices;
		std::vector<Texture> Textures;
		Texture TextureOne;

	private:
		// Render data
		GLuint VAO = 0;
		GLuint VBO = 0;
		GLuint EBO = 0;
	};
}