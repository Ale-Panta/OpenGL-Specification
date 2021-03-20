#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace OpenGL
{
	class Light
	{
	public:
		Light() = default;

	public:
		void CommitToShader(Shader& shader);

	private:
		glm::vec3 m_Position = glm::vec3(10.0f, 0.0f, -4.0f);
		glm::vec3 m_Ambient	= glm::vec3(0.2f, 0.05f, 0.3f);
		glm::vec3 m_Diffuse = glm::vec3(0.5f, 0.8f, 0.6f);
		glm::vec3 m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	};
}