#include "Mesh.h"
#include <SOIL2.h>

namespace OpenGL
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
		: Vertices(vertices), VertexIndices(indices)
	{
		SetupMesh();
	}

	void Mesh::Draw(Shader& shader)
	{
		glUseProgram(shader);

		// Draw Mesh
		// shader.SetUniformInt("uMaterial.diffuse", 0);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, Textures[0].ID);
		// shader.SetUniformInt("uMaterial.specular", 1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, Textures[1].ID);
		glBindVertexArray(VAO);	// Bind
		glDrawElements(GL_TRIANGLES, VertexIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);	// Unbind
	}

	void Mesh::SetupMesh()
	{
		// Textures.push_back(diffuse);
		// Textures.push_back(specular);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, VertexIndices.size() * sizeof(GLuint), &VertexIndices[0], GL_STATIC_DRAW);

		// Vertex position.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		// Vertex normals.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));

		// Vertex texture coords.
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));

		// Vertex color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Color));

		glBindVertexArray(0);
	}
}