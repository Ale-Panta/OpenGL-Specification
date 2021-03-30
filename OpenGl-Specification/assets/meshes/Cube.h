#pragma once

#include "../../src/Mesh.h"

#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace OpenGL
{
	class Cube : public Mesh
	{
	public:
		Cube();

	public:
		virtual void Draw(Shader& shader) override;

	protected:
		virtual void SetupMesh() override;

	private:
		TextureCubeMap* m_CubeMap = nullptr;
		GLuint TextureID;

		float VertexPositions[108] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f
		};

		float m_CubeTexCoord[72] = {
			1.00f, 0.66f, 1.00f, 0.33f, 0.75f, 0.33f,// back face lower right
			0.75f, 0.33f, 0.75f, 0.66f, 1.00f, 0.66f,// back face upper left
			0.75f, 0.33f, 0.50f, 0.33f, 0.75f, 0.66f,// right face lower right
			0.50f, 0.33f, 0.50f, 0.66f, 0.75f, 0.66f,// right face upper left
			0.50f, 0.33f, 0.25f, 0.33f, 0.50f, 0.66f,// front face lower right
			0.25f, 0.33f, 0.25f, 0.66f, 0.50f, 0.66f,// front face upper left
			0.25f, 0.33f, 0.00f, 0.33f, 0.25f, 0.66f,// left face lower right
			0.00f, 0.33f, 0.00f, 0.66f, 0.25f, 0.66f,// left face upper left
			0.25f, 0.33f, 0.50f, 0.33f, 0.50f, 0.00f,// bottom face upper right
			0.50f, 0.00f, 0.25f, 0.00f, 0.25f, 0.33f,// bottom face lower left
			0.25f, 1.00f, 0.50f, 1.00f, 0.50f, 0.66f,// top face upper right
			0.50f, 0.66f, 0.25f, 0.66f, 0.25f, 1.00f// top face lower left
		};;
	};
}