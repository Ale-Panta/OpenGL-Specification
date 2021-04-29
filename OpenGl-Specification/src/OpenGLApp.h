#pragma once
#include "Scene.h"

#include <string>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>	// Must be the last one to be included.
#include <glm/glm.hpp>
#include <stack>

namespace OpenGL
{
	class OpenGLApp
	{
	public:
		OpenGLApp() = default;
		OpenGLApp(const char* name);
		OpenGLApp(const char* name, int width, int height);

	public:
		/** Initialize the context window */
		bool InitWindow();

		/**
		 * Must be called after InitWindow().
		 * Initialize glew. It retrieve the function to communicate with GPU.
		 */
		bool IntiOpenGL();

		/**
		 * Generic OpenGL's running loop.
		 * Here are processed Display, glfwSwapBuffers and glfwPollEvents
		 */
		void Run();

	private:
		/** Exit the OpenGL context properly */
		void Terminate();

	private:
		bool m_IsInitializedProperly = false;

		// Context
		GLFWwindow* m_Context = nullptr;
		std::string m_Name = "OpenGL Default Demo Window";
		int m_Width = 800;
		int m_Height = 800;
		float m_AspectRatio = 1.0f;

		Scene* m_ActiveScene = NULL;
	};
}