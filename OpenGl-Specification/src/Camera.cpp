#include <GL/glew.h>
#include "Camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace OpenGL
{
	Camera::Camera(vec4 initialPosition, float fovy, float aspectRatio, float near, float far)
		: m_WorldPos(vec4(initialPosition))
	{
		m_ViewMat = translate(mat4(1.0f), vec3(-m_WorldPos));
		m_ProjMat = perspective(fovy, aspectRatio, near, far);
	}

	void Camera::UpdateUniformBlock(GLuint ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::M),			sizeof(mat4), value_ptr(GetModelMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::V),			sizeof(mat4), value_ptr(m_ViewMat));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::P),			sizeof(mat4), value_ptr(m_ProjMat));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::Position),	sizeof(vec4), value_ptr(m_WorldPos));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}