#pragma once
#include "../../src/Mesh.h"

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace OpenGL
{
	class Sphere : public Mesh
	{
	public:
		Sphere(int precision = 48);

	private:
		float toRadians(float degrees);
	};
}