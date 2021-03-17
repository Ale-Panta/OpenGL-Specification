#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

namespace OpenGL
{

	Camera::Camera(glm::vec3 initialPosition, float speed, bool setLookAt /*= false*/)
		: m_Position(initialPosition), m_Speed(speed), m_IsLookingAt(setLookAt)
	{
		m_View = glm::translate(glm::mat4(1.0f), -m_Position);
	}

	void Camera::ProcessInput(GLFWwindow* context)
	{
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float actualSpeed = m_Speed * deltaTime;

		if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
			m_Position += actualSpeed * m_Front;
		if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
			m_Position -= actualSpeed * m_Front;
		if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
			m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * actualSpeed;
		if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
			m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * actualSpeed;

		if (!m_IsLookingAt)
		{
			m_View = glm::translate(glm::mat4(1.0f), -m_Position);
		}
		else
		{
			m_View = glm::lookAt(m_Position, m_Target, m_Up);
		}
	}

	void Camera::SetTargetToLookAt(glm::vec3 targetPosition)
	{
		m_Target = targetPosition;
		m_View = glm::lookAt(m_Position, m_Target, m_Up);
	}
}