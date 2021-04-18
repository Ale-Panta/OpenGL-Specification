#include "Cube.h"
#include "SOIL2.h"

using namespace glm;

namespace OpenGL
{
	Cube::Cube()
	{
		for (size_t i = 0; i < 24; i++)
		{
			Vertices.push_back(Vertex());
		}

		// Define the coordinates of each corner of the cube
		vec3 c[8] =
		{
			vec3(- 1.0f, - 1.0f, + 1.0f),
			vec3(+ 1.0f, - 1.0f, + 1.0f),
			vec3(+ 1.0f, - 1.0f, - 1.0f),
			vec3(- 1.0f, - 1.0f, - 1.0f),

			vec3(- 1.0f, + 1.0f, + 1.0f),
			vec3(+ 1.0f, + 1.0f, + 1.0f),
			vec3(+ 1.0f, + 1.0f, - 1.0f),
			vec3(- 1.0f, + 1.0f, - 1.0f)
		};

		// Define the vertices that the cube is composed of:
		// I have used 24 vertices (4 vertices per side).
		// This is because I want the vertices of each side to have separate normals.

		// Bottom
		Vertices[0].Position = c[0]; 
		Vertices[1].Position = c[1]; 
		Vertices[2].Position = c[2]; 
		Vertices[3].Position = c[3];
		// Left
		Vertices[4].Position = c[7]; 
		Vertices[5].Position = c[4]; 
		Vertices[6].Position = c[0]; 
		Vertices[7].Position = c[3];
		// Front
		Vertices[8].Position = c[4]; 
		Vertices[9].Position = c[5]; 
		Vertices[10].Position = c[1];
		Vertices[11].Position = c[0];
		// Back
		Vertices[12].Position = c[6]; 
		Vertices[13].Position = c[7]; 
		Vertices[14].Position = c[3]; 
		Vertices[15].Position = c[2];
		// Right
		Vertices[16].Position = c[5]; 
		Vertices[17].Position = c[6]; 
		Vertices[18].Position = c[2]; 
		Vertices[19].Position = c[1];
		// Top
		Vertices[20].Position = c[7]; 
		Vertices[21].Position = c[6]; 
		Vertices[22].Position = c[5]; 
		Vertices[23].Position = c[4];

		// Define each vertex's tangent
		for (size_t i = 4; i < 24; i += 4)
		{
			vec3 tangent = Vertices[i - 3].Position - Vertices[i - 4].Position;

			for (size_t j = 0; j < 4; j++)
			{
				Vertices[i - j].Tangent = tangent;
			}
		}

		// Define each vertex's normal

		// Bottom
		Vertices[0].Normal = vec3(0.0, -1.0f, 0.0f); 
		Vertices[1].Normal = vec3(0.0, -1.0f, 0.0f); 
		Vertices[2].Normal = vec3(0.0, -1.0f, 0.0f); 
		Vertices[3].Normal = vec3(0.0, -1.0f, 0.0f);

		// Left
		Vertices[4].Normal = vec3(-1.0, 0.0f, 0.0f);
		Vertices[5].Normal = vec3(-1.0, 0.0f, 0.0f);
		Vertices[6].Normal = vec3(-1.0, 0.0f, 0.0f);
		Vertices[7].Normal = vec3(-1.0, 0.0f, 0.0f);

		// Front
		Vertices[8].Normal = vec3(0.0, 0.0f, 1.0f);
		Vertices[9].Normal = vec3(0.0, 0.0f, 1.0f);
		Vertices[10].Normal =vec3(0.0, 0.0f, 1.0f);
		Vertices[11].Normal =vec3(0.0, 0.0f, 1.0f);

		// Back
		Vertices[12].Normal = vec3(0.0, 0.0f, -1.0f);
		Vertices[13].Normal = vec3(0.0, 0.0f, -1.0f);
		Vertices[14].Normal = vec3(0.0, 0.0f, -1.0f);
		Vertices[15].Normal = vec3(0.0, 0.0f, -1.0f);

		// Right
		Vertices[16].Normal = vec3(1.0, 0.0f, 0.0f);
		Vertices[17].Normal = vec3(1.0, 0.0f, 0.0f);
		Vertices[18].Normal = vec3(1.0, 0.0f, 0.0f);
		Vertices[19].Normal = vec3(1.0, 0.0f, 0.0f);

		// Top
		Vertices[20].Normal = vec3(0.0, 1.0f, 0.0f);
		Vertices[21].Normal = vec3(0.0, 1.0f, 0.0f);
		Vertices[22].Normal = vec3(0.0, 1.0f, 0.0f);
		Vertices[23].Normal = vec3(0.0, 1.0f, 0.0f);

		// Define each vetex uv coordinates

		// Bottom
		Vertices[0].TexCoords = vec2(1.0, 1.0f);
		Vertices[1].TexCoords = vec2(0.0, 1.0f);
		Vertices[2].TexCoords = vec2(0.0, 0.0f);
		Vertices[3].TexCoords = vec2(1.0, 0.0f);
											  
		// Left								  
		Vertices[4].TexCoords = vec2(1.0, 1.0f);
		Vertices[5].TexCoords = vec2(0.0, 1.0f);
		Vertices[6].TexCoords = vec2(0.0, 0.0f);
		Vertices[7].TexCoords = vec2(1.0, 0.0f);
											  
		// Front							  
		Vertices[8].TexCoords = vec2(1.0, 1.0f);
		Vertices[9].TexCoords = vec2(0.0, 1.0f);
		Vertices[10].TexCoords = vec2(0.0, 0.0f);
		Vertices[11].TexCoords = vec2(1.0, 0.0f);
											  
		// Back								  
		Vertices[12].TexCoords = vec2(1.0, 1.0f);
		Vertices[13].TexCoords = vec2(0.0, 1.0f);
		Vertices[14].TexCoords = vec2(0.0, 0.0f);
		Vertices[15].TexCoords = vec2(1.0, 0.0f);
											  
		// Right							  
		Vertices[16].TexCoords = vec2(1.0, 1.0f);
		Vertices[17].TexCoords = vec2(0.0, 1.0f);
		Vertices[18].TexCoords = vec2(0.0, 0.0f);
		Vertices[19].TexCoords = vec2(1.0, 0.0f);
											  
		// Top								  
		Vertices[20].TexCoords = vec2(1.0, 1.0f);
		Vertices[21].TexCoords = vec2(0.0, 1.0f);
		Vertices[22].TexCoords = vec2(0.0, 0.0f);
		Vertices[23].TexCoords = vec2(1.0, 0.0f);

		//End Function

		VertexIndices.push_back(3);
		VertexIndices.push_back(1);
		VertexIndices.push_back(0);

		VertexIndices.push_back(3);
		VertexIndices.push_back(2);
		VertexIndices.push_back(1);

		VertexIndices.push_back(7);
		VertexIndices.push_back(5);
		VertexIndices.push_back(4);

		VertexIndices.push_back(7);
		VertexIndices.push_back(6);
		VertexIndices.push_back(5);

		VertexIndices.push_back(11);
		VertexIndices.push_back(9);
		VertexIndices.push_back(8);

		VertexIndices.push_back(11);
		VertexIndices.push_back(10);
		VertexIndices.push_back(9);

		VertexIndices.push_back(15);
		VertexIndices.push_back(13);
		VertexIndices.push_back(12);

		VertexIndices.push_back(15);
		VertexIndices.push_back(14);
		VertexIndices.push_back(13);

		VertexIndices.push_back(19);
		VertexIndices.push_back(17);
		VertexIndices.push_back(16);

		VertexIndices.push_back(19);
		VertexIndices.push_back(18);
		VertexIndices.push_back(17);

		VertexIndices.push_back(23);
		VertexIndices.push_back(21);
		VertexIndices.push_back(20);


		VertexIndices.push_back(23);
		VertexIndices.push_back(22);
		VertexIndices.push_back(21);

		SetupMesh();
	}
}