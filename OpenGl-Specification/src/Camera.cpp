#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

using namespace glm;

namespace OpenGL
{

	Camera::Camera(vec3 initialPosition, float speed, bool setLookAt /*= false*/)
		: m_Position(initialPosition), m_Speed(speed)
	{
		m_View = glm::translate(glm::mat4(1.0f), -m_Position);
	}

	void Camera::ProcessInput(GLFWwindow* context)
	{
		float deltaTime = 0.0f;	// Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float actualSpeed = m_Speed * deltaTime;

		if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
			m_Position += actualSpeed * m_Forward;
		if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
			m_Position -= actualSpeed * m_Forward;
		if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
			m_Position -= m_Right * actualSpeed;
		if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
			m_Position += m_Right * actualSpeed;


		if (glfwGetMouseButton(context, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !m_IsMouseButtonPressed)
		{
			glfwGetCursorPos(context, &m_LastXPos, &m_LastYPos);
			m_IsMouseButtonPressed = true;
		}

		if (glfwGetMouseButton(context, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwGetCursorPos(context, &m_XPos, &m_YPos);

			vec2 pos = vec2((float)m_XPos - (float)m_LastXPos, (float)m_LastYPos - (float)m_YPos);

			ProcessRotation(pos.x, pos.y);
		}

		if (glfwGetMouseButton(context, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			m_LastYaw = m_Yaw;
			m_LastPitch = m_Pitch;
			m_IsMouseButtonPressed = false;
		}

		m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}


	void Camera::ProcessRotation(float xOffset, float yOffset, GLboolean constraintPitch /*= true*/)
	{
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;

		m_Yaw = xOffset + m_LastYaw;
		m_Pitch = yOffset + m_LastPitch;

		if (constraintPitch)
		{
			m_Pitch = clamp(m_Pitch, -89.0f, 89.0f);
		}

		UpdateCameraVectors();
	}


	void Camera::ProcessZoom(float yOffset)
	{
		m_Zoom -= (float)yOffset;

		m_Zoom = clamp(m_Zoom, 1.0f, 45.0f);
	}

	void Camera::UpdateCameraVectors()
	{
		// Calculate the new forward vector
		vec3 front;
		front.x = cos(radians(m_Yaw)) * cos(radians(m_Pitch));
		front.y = sin(radians(m_Pitch));
		front.z = sin(radians(m_Yaw)) * cos(radians(m_Pitch));
		m_Forward = normalize(front);

		// Recalculate the up and right vectors
		m_Right = normalize(cross(m_Forward, vec3(0.0f, 1.0f, 0.0f)));
		m_Up	= normalize(cross(m_Right, m_Forward));
	}
}