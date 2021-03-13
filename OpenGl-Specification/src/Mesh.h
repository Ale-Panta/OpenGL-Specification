#pragma once

#include "Shader.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLM/ext/matrix_float4x2_precision.hpp>

namespace OpenGL
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec3 Normal;
	};

	struct Texture
	{
		unsigned int ID;
		std::string type;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	public:
		void Draw(Shader& shader);

	private:
		void SetupMesh();

	public:
		// Mesh data
		std::vector<Vertex> Vertices;
		std::vector<GLuint> Indices;
		std::vector<Texture> Textures;

	private:
		// Render data
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
	};
}