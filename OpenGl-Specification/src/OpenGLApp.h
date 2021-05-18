#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <stack>
#include <string>
#include <memory>

#include "Scene.h"


namespace OpenGL
{
	class OpenGLApp
	{
	public:
		OpenGLApp() = default;

		/**
		 * OpenGLApp constructor.
		 * @param name - the name of the application.
		 * @param scene - the scene asset to render.
		 */
		OpenGLApp(const char* name, IScene* scene);

		/**
		 * OpenGLApp constructor.
		 * @param name, the name of the application.
		 * @param width, the size width of the window.
		 * @param height, the size height of the window.
		 * @param scene, the scene asset to render.
		 */
		OpenGLApp(const char* name, int width, int height, IScene* scene);

		/**
		 * OpenGLApp destructor.
		 */
		~OpenGLApp();

	public:
		/** 
		 * Initialize the context window.
		 */
		void InitWindow();

		/**
		 * Initialize glew. It retrieve the function to communicate with GPU.
		 * Must be called after InitWindow().
		 */
		void IntiOpenGL();

		/**
		 * Generic OpenGL's running loop.
		 * Here are processed Display, glfwSwapBuffers and glfwPollEvents
		 */
		void Run();

	private:
		/** Track the initialization status between stages. */
		bool m_IsInitializedProperly = false;

		/** Window ptr of the application. */
		GLFWwindow* m_Context = nullptr;

		/** Name of the window. */
		std::string m_Name = "OpenGL Default Demo Window";

		/** Window width. */
		int m_Width = 600;

		/** Window height. */
		int m_Height = 600;

		/** Window aspect ratio. */
		float m_AspectRatio = 1.0f;

		/** Scene ptr asset to render. */
		IScene* m_ActiveScene = NULL;
	};
}