#pragma once

#include "../../src/Mesh.h"

namespace OpenGL
{
	class Plane : public Mesh
	{
	public:
		Plane();

		void Draw(Shader& shader) override;

	};
}