#include "Camera.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace OpenGL
{
	Camera::Camera(vec4 initialPosition, float fovy, float aspectRatio, float near, float far)
		: WorldPos(vec4(initialPosition))
	{
		ViewMat = lookAt(vec3(WorldPos), vec3(0.0f, -1.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		// ProjMat = perspective(fovy, aspectRatio, near, far);
		ProjMat = frustum(-aspectRatio, aspectRatio, -1.0f, 1.0f, 1.0f, 800.0f);
	}

	void Camera::UpdateUniformBlock(GLuint ubo)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::Model),			sizeof(mat4), value_ptr(GetModelMat()));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::View),			sizeof(mat4), value_ptr(ViewMat));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::Projection),	sizeof(mat4), value_ptr(ProjMat));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::Position),		sizeof(vec4), value_ptr(WorldPos));
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(CameraProperties, CameraProperties::Direction),		sizeof(vec4), value_ptr(GetForward()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Camera::SetWorldPos(glm::vec4 worldPos)
	{
		WorldPos = worldPos;
		ViewMat = lookAt(vec3(WorldPos), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	}

}