#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace OpenGL
{
	class Camera
	{
	public:
		Camera(glm::vec3 initialPosition, float speed, bool setLookAt = false);

	public:
		void ProcessInput(GLFWwindow* context);
		void ProcessRotation(float xOffset, float yOffset, GLboolean constraintPitch = true);
		void ProcessZoom(float yOffset);

		const glm::mat4& GetViewMatrix() const { return m_View; }
		const glm::mat4& GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), m_Position); }
		const glm::mat4& GetProjMatrix() const { return m_Projection; }
		const glm::vec3& GetPosition() const { return m_Position; }

	private:
		void UpdateCameraVectors();

	private:
		glm::mat4 m_View;
		glm::mat4 m_Projection = glm::perspective(1.0472f, 1.0f, 0.1f, 100.0f);

		glm::vec3 m_Position	= glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_Forward		= glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up			= glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right		= glm::vec3(1.0f, 0.0f, 0.0f);

		// Euler angles
		float m_Yaw		= -90.0f;
		float m_Pitch	= 0.0f;

		float m_LastYaw		= 0.0f;
		float m_LastPitch	= 0.0f;

		// Camera settings
		float m_Speed				= 2.5f;
		float m_MouseSensitivity	= 0.2f;
		float m_Zoom				= 45.0f;

		bool m_IsMouseButtonPressed = false;
		double m_LastXPos	= 0.0f;
		double m_LastYPos	= 0.0f;
		double m_XPos		= 0.0f;
		double m_YPos		= 0.0f;
	};
}