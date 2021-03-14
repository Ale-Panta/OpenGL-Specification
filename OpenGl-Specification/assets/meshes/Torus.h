#pragma once
#include "../../src/Mesh.h"

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace OpenGL
{
	class Torus : public Mesh
	{
	public:
		Torus(float innerRadius = 0.2f, float outerRadius = 0.5f, int precision = 48);

	private:
		float toRadians(float degrees);

	private:
		std::vector<glm::vec3> STangents;
		std::vector<glm::vec3> TTangents;
	};
}