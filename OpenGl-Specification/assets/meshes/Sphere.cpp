#include "Sphere.h"

#include <iostream>

using namespace std;
using namespace glm;

namespace OpenGL
{
	Sphere::Sphere(int precision)
	{
		for (int i = 0; i < (precision + 1) * (precision + 1); i++) 
		{ 
			Vertices.push_back({ 
				glm::vec3(), // Position
				glm::vec2(), // TexCoord or UV
				glm::vec3(), // Normal
				glm::vec3(), // Tangent
				glm::vec3()  // Color
				}); 
		}

		for (int i = 0; i < precision * precision * 6; i++) 
		{ 
			VertexIndices.push_back(0); 
		}

		// Calculate triangle vertices
		for (int i = 0; i <= precision; i++)
		{
			for (int j = 0; j <= precision; j++)
			{
				float y = (float)cos(toRadians(180.0f - i * 180.0f / precision));
				float x = -(float)cos(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));
				float z = (float)sin(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));

				Vertices[i * (precision + 1) + j].Position = glm::vec3(x, y, z);
				Vertices[i * (precision + 1) + j].TexCoords = glm::vec2(((float)j / precision), ((float)i / precision));
				Vertices[i * (precision + 1) + j].Normal = glm::vec3(x, y, z);
				
				if ((x == 0) && (y == 1) && (z == 0) || (x == 0) && (y == -1) && (z == 0))
					Vertices[i * (precision + 1) + j].Tangent = glm::vec3(0.0f, 0.0f, -1.0f);
				else
					Vertices[i * (precision + 1) + j].Tangent = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
			}
		}

		// Calculate triangle indices
		for (int i = 0; i < precision; i++)
		{
			for (int j = 0; j < precision; j++)
			{
				VertexIndices[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
				VertexIndices[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
				VertexIndices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
				VertexIndices[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
				VertexIndices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
				VertexIndices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
			}
		}

		SetupMesh();
	}

	float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
}