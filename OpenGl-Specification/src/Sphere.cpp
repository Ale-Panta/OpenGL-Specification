#include "Sphere.h"

#include <iostream>

using namespace std;
using namespace glm;

namespace OpenGL
{
	Sphere::Sphere()
	{
		Init(48);
	}

	Sphere::Sphere(int precision)
	{
		Init(precision);
	}

	void Sphere::Init(int precision)
	{
		m_VerticesNum = (precision + 1) * (precision + 1);
		m_IndicesNum = precision * precision * 6;

		for (int i = 0; i < m_VerticesNum; i++) { m_Vertices.push_back(glm::vec3()); }
		for (int i = 0; i < m_VerticesNum; i++) { m_TexCoords.push_back(glm::vec2()); }
		for (int i = 0; i < m_VerticesNum; i++) { m_Normals.push_back(glm::vec3()); }
		for (int i = 0; i < m_IndicesNum; i++)	{ m_Indicies.push_back(0); }

		// Calculate triangle vertices
		for (int i = 0; i <= precision; i++)
		{
			for (int j = 0; j <= precision; j++)
			{
				float y =  (float)cos(toRadians(180.0f - i * 180.0f / precision));
				float x = -(float)cos(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));
				float z =  (float)sin(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));

				m_Vertices	[i * (precision + 1) + j] = glm::vec3(x, y, z);
				m_TexCoords	[i * (precision + 1) + j] = glm::vec2(((float)j / precision), ((float)i / precision));
				m_Normals	[i * (precision + 1) + j] = glm::vec3(x, y, z);
			}
		}

		// Calculate triangle indices
		for (int i = 0; i < precision; i++)
		{
			for (int j = 0; j < precision; j++)
			{
				m_Indicies[6 * (i * precision + j) + 0] = i * (precision + 1) + j;
				m_Indicies[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
				m_Indicies[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
				m_Indicies[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
				m_Indicies[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
				m_Indicies[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
			}
		}
	}

	float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

	// Accessors
	int Sphere::GetVerticesCount() { return m_VerticesNum; }
	int Sphere::GetIndicesCount() { return m_IndicesNum; }
	std::vector<int> Sphere::GetIndices() { return m_Indicies; }
	std::vector<glm::vec3> Sphere::GetVertices() { return m_Vertices; }
	std::vector<glm::vec2> Sphere::GetTexCoords() { return m_TexCoords; }
	std::vector<glm::vec3> Sphere::GetNormals() { return m_Normals; }
}