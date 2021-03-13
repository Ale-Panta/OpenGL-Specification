#include "OpenGLApp.h"

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
		m_CameraPosition = glm::vec3(0.0f, 1.0f, 7.0f);

		m_AspectRatio = (float)m_Width / (float)m_Height;
		//pMat = perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degree

		float vertices[24] =
		{
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f
		};

		float texCoords[4 * 2] =
		{
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		float normals[6 * 3] =
		{
			0, 0, 1,
			1, 0, 0,
			0, 0, -1,
			-1, 0, 0,
			0, 1, 0,
			0, -1, 0
		};

		m_Indices = new int[6 * 6]
		{
			0, 1, 3, 3, 1, 2,
			1, 5, 2, 2, 5, 6,
			5, 4, 6, 6, 4, 7,
			4, 0, 7, 7, 0, 3,
			3, 2, 7, 7, 2, 6,
			4, 5, 0, 0, 5, 1
		};

		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);
		glGenBuffers(4, vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * 4, m_Indices, GL_STATIC_DRAW);

		m_Shader = std::make_shared<Shader>("assets/shaders/vertShader.glsl", "assets/shaders/fragShader.glsl");
	}

	void OpenGLApp::Display(GLFWwindow* context, double currentTime)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glUseProgram(*m_Shader);

		// Build perspective matrix
		m_ProjectionMatrix = glm::perspective(1.0472f, m_AspectRatio, 0.1f, 100.0f);

		// Build model view matrix with the stack.
		m_ModelViewStack.push(glm::translate(glm::mat4(1.0f), -m_CameraPosition));
		m_ModelViewStack.push(m_ModelViewStack.top());
		m_ModelViewStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		m_ModelViewStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(currentTime * 0.1f, currentTime * 0.4f, currentTime * 0.23f));

		// Copy perspective and MV matrices to corresponding uniform variables.
		m_Shader->UploadUniformMatrix4fv("u_pMat", m_ProjectionMatrix);
		m_Shader->UploadUniformMatrix4fv("u_mvMat", m_ModelViewStack.top());

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

		// Adjust OpenGL settings and draw model.
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		m_ModelViewStack.empty();
	}

	void OpenGLApp::WindowReshapeCallback(GLFWwindow* window, int newWidth, int newHeight)
	{
		//// Build perspective matrix.
		//m_AspectRatio = (float)newWidth / (float)newHeight;
		//glViewport(0, 0, newWidth, newHeight);
		//glm::perspective(1.0472f, m_AspectRatio, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.
	}
}