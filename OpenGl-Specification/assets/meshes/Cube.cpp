#include "Cube.h"
#include "SOIL2.h"

using namespace glm;

namespace OpenGL
{
	Cube::Cube()
	{
		for (size_t i = 0; i < 36; i++)
		{
			Vertices.push_back(Vertex());
		}
			
		Vertices[0].Position = vec3(-1.0f, 1.0f, -1.0f);
		Vertices[1].Position = vec3(-1.0f, -1.0f, -1.0f);
		Vertices[2].Position = vec3(1.0f, -1.0f, -1.0f);
		Vertices[3].Position = vec3(1.0f, -1.0f, -1.0f);
		Vertices[4].Position = vec3(1.0f, 1.0f, -1.0f);
		Vertices[5].Position = vec3(-1.0f, 1.0f, -1.0f);

		Vertices[6].Position = vec3(-1.0f, -1.0f, 1.0f);
		Vertices[7].Position = vec3(-1.0f, -1.0f, -1.0f);
		Vertices[8].Position = vec3(-1.0f, 1.0f, -1.0f);
		Vertices[9].Position = vec3(-1.0f, 1.0f, -1.0f);
		Vertices[10].Position = vec3(-1.0f, 1.0f, 1.0f);
		Vertices[11].Position = vec3(-1.0f, -1.0f, 1.0f);

		Vertices[12].Position = vec3(1.0f, -1.0f, -1.0f);
		Vertices[13].Position = vec3(1.0f, -1.0f, 1.0f);
		Vertices[14].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[15].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[16].Position = vec3(1.0f, 1.0f, -1.0f);
		Vertices[17].Position = vec3(1.0f, -1.0f, -1.0f);
		
		Vertices[18].Position = vec3(-1.0f, -1.0f, 1.0f);
		Vertices[19].Position = vec3(-1.0f, 1.0f, 1.0f);
		Vertices[20].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[21].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[22].Position = vec3(1.0f, -1.0f, 1.0f);
		Vertices[23].Position = vec3(-1.0f, -1.0f, 1.0f);
		
		Vertices[24].Position = vec3(-1.0f, 1.0f, -1.0f);
		Vertices[25].Position = vec3(1.0f, 1.0f, -1.0f);
		Vertices[26].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[27].Position = vec3(1.0f, 1.0f, 1.0f);
		Vertices[28].Position = vec3(-1.0f, 1.0f, 1.0f);
		Vertices[29].Position = vec3(-1.0f, 1.0f, -1.0f);
		
		Vertices[30].Position = vec3(-1.0f, -1.0f, -1.0f);
		Vertices[31].Position = vec3(-1.0f, -1.0f, 1.0f);
		Vertices[32].Position = vec3(1.0f, -1.0f, -1.0f);
		Vertices[33].Position = vec3(1.0f, -1.0f, -1.0f);
		Vertices[34].Position = vec3(-1.0f, -1.0f, 1.0f);
		Vertices[35].Position = vec3(1.0f, -1.0f, 1.0f);
		
		//Begin Function CalculateSurfaceNormal(Input Triangle) Returns Vector

		//	Set Vector U to(Triangle.p2 minus Triangle.p1)
		//	Set Vector V to(Triangle.p3 minus Triangle.p1)

		//	Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
		//	Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
		//	Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

		//	Returning Normal

		//End Function

		SetupMesh();
	}

	//void Cube::Draw(Shader& shader)
	//{
	//	glUseProgram(shader);

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_CUBE_MAP, *mCubeMap);

	//	glEnable(GL_CULL_FACE);
	//	glFrontFace(GL_CCW);

	//	glDisable(GL_DEPTH_TEST);
	//	glBindVertexArray(VAO);	// Bind
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	glBindVertexArray(0);	// Unbind
	//	glEnable(GL_DEPTH_TEST);
	//}

	void Cube::SetupMesh()
	{
		mCubeMap = new TextureCubeMap(
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
		);

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