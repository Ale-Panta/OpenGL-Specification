#pragma once
#include "Sphere.h"

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace OpenGL
{
	class Sphere
	{
	public:
		Sphere();
		Sphere(int precision);

	public:
		int GetVerticesCount();
		int GetIndicesCount();

		std::vector<int> GetIndices();
		std::vector<glm::vec3> GetVertices();
		std::vector<glm::vec2> GetTexCoords();
		std::vector<glm::vec3> GetNormals();

	private:
		void Init(int precision);
		float toRadians(float degrees);

	private:
		int m_VerticesNum = 0;
		int m_IndicesNum = 0;

		std::vector<int> m_Indicies;
		std::vector<glm::vec3> m_Vertices;
		std::vector<glm::vec2> m_TexCoords;
		std::vector<glm::vec3> m_Normals;
	};
}