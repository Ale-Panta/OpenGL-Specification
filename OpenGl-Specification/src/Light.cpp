#include "Light.h"

namespace OpenGL
{
	void Light::CommitToShader(Shader& shader)
	{
		shader.SetUniformVec3("uLight.position", m_Position);
		shader.SetUniformVec3("uLight.ambient", m_Ambient);
		shader.SetUniformVec3("uLight.diffuse", m_Diffuse);
		shader.SetUniformVec3("uLight.specular", m_Specular);
	}
}