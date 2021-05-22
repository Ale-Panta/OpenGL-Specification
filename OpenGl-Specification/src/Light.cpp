#include "Light.h"

#include <glm/gtc/type_ptr.hpp>
#include "OpenGLApp.h"

using namespace glm;

namespace OpenGL
{
	Light::Light(vec4 worldPos, vec4 ambientCol, vec4 diffuseCol, vec4 specularCol)
		: WorldPos(worldPos), AmbientCol(ambientCol), DiffuseCol(diffuseCol), SpecularCol(specularCol)
	{
		ViewMat = lookAt(vec3(WorldPos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		ProjMat = frustum(-OpenGLApp::GetApp()->GetAspectRatio(), OpenGLApp::GetApp()->GetAspectRatio(), -1.0f, 1.0f, 1.0f, 800.0f);
	}

	void Light::UpdateUniformBlock(GLuint ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Model),		sizeof(mat4), value_ptr(GetModelMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::View),		sizeof(mat4), value_ptr(GetViewMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Projection),	sizeof(mat4), value_ptr(GetProjMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Position),	sizeof(vec4), value_ptr(WorldPos));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Direction),	sizeof(vec4), value_ptr(GetForward()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Color),		sizeof(vec4), value_ptr(DiffuseCol));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LightProperties, LightProperties::Ambient),		sizeof(vec4), value_ptr(AmbientCol));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Light::SetWorldPos(glm::vec4 worldPos)
	{
		WorldPos = worldPos;
		ViewMat = lookAt(vec3(WorldPos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	}
}