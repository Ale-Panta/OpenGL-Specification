#include "Cube.h"
#include "SOIL2.h"

using namespace glm;

namespace OpenGL
{
	Cube::Cube()
	{
		SetupMesh();
	}

	void Cube::Draw(Shader& shader)
	{
		glUseProgram(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);	// Bind
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);	// Unbind
		glEnable(GL_DEPTH_TEST);
	}

	void Cube::SetupMesh()
	{
		const char* top = "assets/textures/skybox/top.jpg";
		const char* bottom = "assets/textures/skybox/bottom.jpg";
		const char* left = "assets/textures/skybox/left.jpg";
		const char* right = "assets/textures/skybox/right.jpg";
		const char* front = "assets/textures/skybox/front.jpg";
		const char* back = "assets/textures/skybox/back.jpg";
		TextureID = SOIL_load_OGL_cubemap(right, left, top, bottom, front, back, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

		glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
		// reduce seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositions), VertexPositions, GL_STATIC_DRAW);

		// Vertex position.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}
}