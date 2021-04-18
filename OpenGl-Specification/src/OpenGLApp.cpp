#include "OpenGLApp.h"

// Begin scene assets
#include "../assets/scenes/PBRScene.h"
#include "../assets/scenes/ShadowScene.h"
#include "../assets/scenes/HeightMapScene.h"
#include "../assets/scenes/NormalMappingScene.h"
#include "../assets/scenes/EnvironmentMappingScene.h"
// End scene assets

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace OpenGL
{
	OpenGLApp::OpenGLApp(const char* name)
		: m_Name(name)
	{
		m_ActiveScene = new EnvironmentMappingScene();
	}

	OpenGLApp::OpenGLApp(const char* name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
		m_ActiveScene = new PBRScene();
	}

	bool OpenGLApp::InitWindow()
	{
		if (!glfwInit())
		{
			m_IsInitializedProperly = false;
		}

		// Check versions
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		// Declare multi-sample. Must be declared before creating the context.
		glfwWindowHint(GLFW_SAMPLES, 4);

		// Create the window context.
		// Note: Creating the window doesn't make it current context by default.
		m_Context = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), nullptr, nullptr);

		// Mark the window as current context
		glfwMakeContextCurrent(m_Context);

		// Enable multi-samples.
		glEnable(GL_MULTISAMPLE);

		m_IsInitializedProperly = true;
		return m_IsInitializedProperly;
	}

	bool OpenGLApp::IntiOpenGL()
	{
		// glewInit must be called after the creation of the context.
		if (glewInit() != GLEW_OK)
		{
			m_IsInitializedProperly = false;
		}

		// Active swap buffer
		glfwSwapInterval(1);

		// Set up frame buffer
		glfwGetFramebufferSize(m_Context, &m_Width, &m_Height);

		// Set window resize callback
		m_IsInitializedProperly = true;


		return m_IsInitializedProperly;
	}

	void OpenGLApp::Run()
	{
		if (!m_IsInitializedProperly)
		{
			Terminate();
			return;
		}

		m_ActiveScene->BeginScene(m_Context);

		while (!glfwWindowShouldClose(m_Context))
		{
			m_ActiveScene->RenderScene(m_Context, glfwGetTime());
			glfwSwapBuffers(m_Context);			// GLFW are by default double-buffered.
			glfwPollEvents();					// Handle other window-related events.
		}

		Terminate();
	}

	void OpenGLApp::Terminate()
	{
		glfwDestroyWindow(m_Context);
		glfwTerminate();
	}
}