#include "OpenGLApp.h"
#include "../assets/meshes/Sphere.h"
#include "../assets/meshes/Torus.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		// Set camera position
		m_CameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);

		m_AspectRatio = (float)m_Width / (float)m_Height;
		m_ProjectionMatrix = glm::perspective(1.0472f, m_AspectRatio, 0.1f, 1000.0f); // 1.0472 radians = 60 degree

		m_Sphere = std::make_shared<Sphere>();
		m_SphereSecond = std::make_shared<Sphere>();
		m_Torus = std::make_shared<Torus>();
		m_LightSrc = std::make_shared<Sphere>();
		m_Shader = std::make_shared<Shader>("assets/shaders/vertLitShader.glsl", "assets/shaders/fragLitShader.glsl");
		m_LightSrcShader = std::make_shared<Shader>("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
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

		// Bind program to use while drawing.

		// Set lighting uniform
		m_Shader->SetUniformVec3("uLight.position", glm::vec3(sin(currentTime) * 15.0f, 0.0f, cos(currentTime) * 15.0f));
		m_Shader->SetUniformVec3("uLight.ambient", glm::vec3(0.6f, 0.5f, 0.5f));
		m_Shader->SetUniformVec3("uLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
		m_Shader->SetUniformVec3("uLight.specular", glm::vec3(0.7f, 0.5f, 0.2f));

		// Set material uniform
		m_Shader->SetUniformVec3("uMaterial.ambient", glm::vec3(0.5f, 0.65f, 0.0f));
		m_Shader->SetUniformVec3("uMaterial.diffuse", glm::vec3(0.5f));
		m_Shader->SetUniformVec3("uMaterial.specular", glm::vec3(0.2f));
		m_Shader->SetUniformFloat("uMaterial.shininess", 0.7f);

		m_Shader->SetUniformFloat("uTime", (float)currentTime);
		m_Shader->SetUniformVec3("uViewPos", m_CameraPosition);
		m_Shader->SetUniformMatrix4("uProjection", m_ProjectionMatrix);
		m_LightSrcShader->SetUniformMatrix4("uProjection", m_ProjectionMatrix);

		// Build view matrix
		m_ModelViewStack.push(glm::translate(glm::mat4(1.0f), -m_CameraPosition));
		m_Shader->SetUniformMatrix4("uView", m_ModelViewStack.top());
		m_LightSrcShader->SetUniformMatrix4("uView", m_ModelViewStack.top());

		// Define source light
		m_ModelViewStack.push(m_ModelViewStack.top());
		m_ModelViewStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime) * 15.0f, 0.0f, cos(currentTime) * 15.0f));
		m_LightSrcShader->SetUniformMatrix4("uModel", m_ModelViewStack.top());
		m_LightSrc->Draw(*m_LightSrcShader);

		m_ModelViewStack.pop();

		// Define Sphere model
		m_Shader->SetUniformMatrix4("uModel", m_ModelViewStack.top());
		m_Sphere->Draw(*m_Shader);

		// Define Torus model
		m_Shader->SetUniformVec3("uMaterial.ambient", glm::vec3(0.5f));
		m_ModelViewStack.push(m_ModelViewStack.top());
		m_ModelViewStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.5f, 0.0f));
		m_Shader->SetUniformMatrix4("uModel", m_ModelViewStack.top());
		m_Torus->Draw(*m_Shader);

		// Define Torus model
		m_Shader->SetUniformVec3("uMaterial.ambient", glm::vec3(0.5f, 0.5f, 0.7f));
		m_ModelViewStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 1.0f));
		m_Shader->SetUniformMatrix4("uModel", m_ModelViewStack.top());
		m_SphereSecond->Draw(*m_Shader);

		m_ModelViewStack.pop();
		m_ModelViewStack.pop();
	}
}