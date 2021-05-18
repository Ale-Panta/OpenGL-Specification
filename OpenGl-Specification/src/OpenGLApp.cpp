#include "OpenGLApp.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace OpenGL
{
	OpenGLApp::OpenGLApp(const char* name, IScene* scene)
		: m_Name(name)
	{
		m_ActiveScene = scene;
	}

	OpenGLApp::OpenGLApp(const char* name, int width, int height, IScene* scene)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		m_ActiveScene = scene;
	}


	OpenGLApp::~OpenGLApp()
	{
		if (m_ActiveScene)
			delete m_ActiveScene;
	}

	void OpenGLApp::InitWindow()
	{
		if (!glfwInit())
		{
			exit(EXIT_FAILURE);	// Shut down the application
		}

		// Check versions
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		// Declare multi-sample
		glfwWindowHint(GLFW_SAMPLES, 4);

		// Create the window context.
		// Note: Creating the window doesn't make it current context by default.
		m_Context = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), nullptr, nullptr);

		// Mark the window as current context
		glfwMakeContextCurrent(m_Context);

		// Enable multi-samples.
		glEnable(GL_MULTISAMPLE);
	}

	void OpenGLApp::IntiOpenGL()
	{
		if (glewInit() != GLEW_OK)
		{
			exit(EXIT_FAILURE);	// Shut down the application
		}

		// Active swap buffer
		glfwSwapInterval(1);

		// Set up frame buffer
		glfwGetFramebufferSize(m_Context, &m_Width, &m_Height);
	}

	void OpenGLApp::Run()
	{
		m_ActiveScene->BeginScene(m_Context);

		while (!glfwWindowShouldClose(m_Context))
		{
			m_ActiveScene->RenderScene(m_Context, glfwGetTime());
			glfwSwapBuffers(m_Context);
			glfwPollEvents();					// Handle window-related events such as inputs
		}
		
		// Close OpenGL properly
		glfwDestroyWindow(m_Context);
		glfwTerminate();
	}
}