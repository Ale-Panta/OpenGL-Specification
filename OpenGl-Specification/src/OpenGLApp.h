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
		/** 
		 * Initialize the context window
		 * 
		 * @return success or fail
		 */
		bool InitWindow();

		/**
		 * Initialize glew. It retrieve the function to communicate with GPU.
		 * Must be called after InitWindow().
		 * 
		 * @return success or fail
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
		/** Track the initialization status between stages */
		bool m_IsInitializedProperly = false;

		/** Window ptr of the application */
		GLFWwindow* m_Context = nullptr;

		/** Name of the window */
		std::string m_Name = "OpenGL Default Demo Window";

		/** Window width */
		int m_Width = 600;

		/** Window height */
		int m_Height = 600;

		/** Window aspect ratio */
		float m_AspectRatio = 1.0f;

		/** Scene ptr asset to render */
		Scene* m_ActiveScene = NULL;
	};
}