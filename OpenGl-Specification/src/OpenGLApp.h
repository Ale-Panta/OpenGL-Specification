#pragma once

#include "Mesh.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Material.h"

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
		 * Initialize the context window.
		 */
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
		/**
		 * Exit the OpenGL context properly.
		 */
		void Terminate();

		/**
		 * Initialize inside this method all additional properties such as: 
		 * model creation, 
		 * binding shader program and so on.
		 * It's used also to fill the vao and vbo.
		 */
		void PreDisplay(GLFWwindow* window);

		/**
		 * Here is where the geometry is drown and re-drown again and again.
		 * Usually you want to draw here all the geometry in the vbo.
		 */
		void Display(GLFWwindow* window, double currentTime);

	private:
		bool m_IsInitializedProperly = false;

		// Context
		GLFWwindow* m_Context = nullptr;
		std::string m_Name = "OpenGL Default Demo Window";
		int m_Width = 1280;
		int m_Height = 720;
		float m_AspectRatio = 1.0f;

		// Rendering
		std::shared_ptr<Camera> m_Camera		= NULL;
		std::shared_ptr<Mesh> m_Torus			= NULL;
		std::shared_ptr<Material> m_Material	= NULL;
		std::shared_ptr<Light> m_Light			= NULL;
		std::shared_ptr<Shader> m_LitShader		= NULL;
	};
}