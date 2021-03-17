#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

namespace OpenGL
{
	class Camera
	{
	public:
		Camera(glm::vec3 initialPosition, float speed, bool setLookAt = false);

	public:
		void ProcessInput(GLFWwindow* context);
		void SetTargetToLookAt(glm::vec3 targetPosition);
		const glm::mat4& GetViewMatrix() const { return m_View; }
		const glm::mat4& GetProjMatrix() const { return m_Projection; }

	private:
		float m_Speed = 2.5f;
		glm::vec3 m_Position	= glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_Front		= glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up			= glm::vec3(0.0f, 1.0f, 0.0f);

		bool m_IsLookingAt = false;
		glm::vec3 m_Target	= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 m_View;
		glm::mat4 m_Projection = glm::perspective(1.0472f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	};
}