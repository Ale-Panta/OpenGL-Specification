#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace OpenGL
{
	class Material
	{
	public:
		Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular);

	public:
		void CommitToProgram(Shader& shader);

	private:
		glm::vec3 m_GlobalAmbient = glm::vec3(0.75f, 0.75f, 0.75f);
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
		glm::vec3 m_Shininess = glm::vec3(0.02f, 0.02f, 0.02f);
	};
}