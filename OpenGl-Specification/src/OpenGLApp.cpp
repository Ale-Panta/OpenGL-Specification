#include "OpenGLApp.h"
#include "../assets/meshes/Sphere.h"
#include "../assets/meshes/Torus.h"

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
	}

	OpenGLApp::OpenGLApp(const char* name, int width, int height)
		: m_Name(name), m_Width(width), m_Height(height)
	{
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

		// Create the window context.
		// Note: Creating the window doesn't make it current context by default.
		m_Context = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), nullptr, nullptr);

		// Mark the window as current context
		glfwMakeContextCurrent(m_Context);

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

		// Compute geometry. Here are filled vao and vbo.
		PreDisplay(m_Context);

		while (!glfwWindowShouldClose(m_Context))
		{
			m_Camera->ProcessInput(m_Context);
			Display(m_Context, glfwGetTime());
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

	void OpenGLApp::PreDisplay(GLFWwindow* window)
	{
		m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 4.0f), 0.05f, false);

		// Create geometries
		m_Torus	= std::make_shared<Torus>();

		// Create light source
		m_Light = std::make_shared<Light>();

		// Create materials
		m_Material = std::make_shared<SilverMaterial>();

		// Create shaders
		m_LitShader	= std::make_shared<Shader>("assets/shaders/BlinnPhong/vertBlinnPhongShader.glsl", "assets/shaders/BlinnPhong/fragBlinnPhongShader.glsl");
	}

	void OpenGLApp::Display(GLFWwindow* context, double currentTime)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Adjust OpenGL settings and draw model.
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Enable wireframe mode.
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Set lighting uniform
		m_Light->CommitToShader(*m_LitShader);

		// Set material uniform
		m_Material->CommitToProgram(*m_LitShader);

		m_LitShader->SetUniformFloat("uTime", (float)currentTime);
		m_LitShader->SetUniformMatrix4("uModel", glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
		m_LitShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_LitShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());

		// Define Torus model
		m_Torus->Draw(*m_LitShader);
	}
}