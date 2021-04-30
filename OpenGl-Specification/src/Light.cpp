#include "Light.h"

#include <glm/gtc/type_ptr.hpp>

using namespace glm;

namespace OpenGL
{
	Light::Light(vec4 worldPos, vec4 ambientCol, vec4 diffuseCol, vec4 specularCol)
		: m_WorldPos(worldPos), m_AmbientCol(ambientCol), m_DiffuseCol(diffuseCol), m_SpecularCol(specularCol)
	{
		m_ViewMat = glm::lookAt(vec3(m_WorldPos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		m_ProjMat = glm::ortho(-1, 1, -1, 1);
	}


	void Light::UpdateUniformBlock(GLuint ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Position),	sizeof(vec4), value_ptr(m_WorldPos));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Color),		sizeof(vec4), value_ptr(m_DiffuseCol));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Ambient),		sizeof(vec4), value_ptr(m_AmbientCol));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}