#include "OpenGLApp.h"

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
		m_CameraPosition = glm::vec3(4.0f);

		m_AspectRatio = (float)m_Width / (float)m_Height;
		//pMat = perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degree

		m_Shader = std::make_shared<Shader>("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);
	}

	void OpenGLApp::Display(GLFWwindow* context, double currentTime)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(*m_Shader);

		m_Shader->UploadUniform1f("u_offset", (float)currentTime / 10);

		glPointSize(30.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void OpenGLApp::WindowReshapeCallback(GLFWwindow* window, int newWidth, int newHeight)
	{
		//// Build perspective matrix.
		//m_AspectRatio = (float)newWidth / (float)newHeight;
		//glViewport(0, 0, newWidth, newHeight);
		//glm::perspective(1.0472f, m_AspectRatio, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.
	}
}