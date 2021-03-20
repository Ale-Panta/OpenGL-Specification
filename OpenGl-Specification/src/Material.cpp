#include "MAterial.h"

namespace OpenGL
{
	Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular)
	{

	}

	void Material::CommitToProgram(Shader& shader)
	{
		shader.SetUniformVec3("uMaterial.globalAmbient", m_GlobalAmbient);
		shader.SetUniformVec3("uMaterial.ambient", m_Ambient);
		shader.SetUniformVec3("uMaterial.diffuse", m_Diffuse);
		shader.SetUniformVec3("uMaterial.specular", m_Specular);
	}
}