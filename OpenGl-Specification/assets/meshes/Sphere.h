#pragma once
#include "../../src/Mesh.h"

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