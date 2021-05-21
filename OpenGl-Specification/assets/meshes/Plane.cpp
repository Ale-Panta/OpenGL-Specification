#include "Plane.h"
#include "glm/fwd.hpp"

using namespace glm;

namespace OpenGL
{
	Plane::Plane()
	{
		for (unsigned char i = 0; i < 4; i++)
		{
			Vertices.push_back(Vertex());
		}

		Vertices[0].Position = vec3(-1.0f, +1.0f, +0.0f);
		Vertices[1].Position = vec3(-1.0f, -1.0f, +0.0f);
		Vertices[2].Position = vec3(+1.0f, -1.0f, +0.0f);
		Vertices[3].Position = vec3(+1.0f, +1.0f, +0.0f);

		for (size_t i = 0; i < 4; i++)
			Vertices[i].Tangent = vec3(0.0f, -1.0f, 0.0f);

		Vertices[0].TexCoords = vec2(0.0f, 1.0f);
		Vertices[1].TexCoords = vec2(0.0f, 0.0f);
		Vertices[2].TexCoords = vec2(1.0f, 0.0f);
		Vertices[3].TexCoords = vec2(1.0f, 1.0f);

		Vertices[0].Normal = vec3(0.0f, 0.0f, 1.0f);
		Vertices[1].Normal = vec3(0.0f, 0.0f, 1.0f);
		Vertices[2].Normal = vec3(0.0f, 0.0f, 1.0f);
		Vertices[3].Normal = vec3(0.0f, 0.0f, 1.0f);

		VertexIndices.push_back(0);
		VertexIndices.push_back(1);
		VertexIndices.push_back(2);

		VertexIndices.push_back(2);
		VertexIndices.push_back(3);
		VertexIndices.push_back(0);

		SetupMesh();
	}
}