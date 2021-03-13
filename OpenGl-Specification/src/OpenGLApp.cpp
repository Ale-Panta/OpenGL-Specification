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

		float vertices[] =
		{
			 .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f, // v0,v1,v2,v3 (front)
			 .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f, // v0,v3,v4,v5 (right)
			 .5f, .5f, .5f,   .5f, .5f,-.5f,  -.5f, .5f,-.5f, -.5f, .5f, .5f, // v0,v5,v6,v1 (top)
			-.5f, .5f, .5f,  -.5f, .5f,-.5f,  -.5f,-.5f,-.5f, -.5f,-.5f, .5f, // v1,v6,v7,v2 (left)
			-.5f,-.5f,-.5f,   .5f,-.5f,-.5f,   .5f,-.5f, .5f, -.5f,-.5f, .5f, // v7,v4,v3,v2 (bottom)
			 .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f  // v4,v7,v6,v5 (back)
		};

		float texCoords[] =
		{
			1, 0,   0, 0,   0, 1,   1, 1,               // v0,v1,v2,v3 (front)
			0, 0,   0, 1,   1, 1,   1, 0,               // v0,v3,v4,v5 (right)
			1, 1,   1, 0,   0, 0,   0, 1,               // v0,v5,v6,v1 (top)
			1, 0,   0, 0,   0, 1,   1, 1,               // v1,v6,v7,v2 (left)
			0, 1,   1, 1,   1, 0,   0, 0,               // v7,v4,v3,v2 (bottom)
			0, 1,   1, 1,   1, 0,   0, 0                // v4,v7,v6,v5 (back
		};

		float normals[] =
		{
			 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,  // v0,v1,v2,v3 (front)
			 1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,  // v0,v3,v4,v5 (right)
			 0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,  // v0,v5,v6,v1 (top)
			-1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  // v1,v6,v7,v2 (left)
			 0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,  // v7,v4,v3,v2 (bottom)
			 0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1   // v4,v7,v6,v5 (back)
		};

		m_Indices = new int[]
		{
			 0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
			 4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
			 8, 9,10,  10, 11, 8,   // v0-v5-v6, v6-v1-v0 (top)
			12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
			16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
			20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
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


		// Build perspective matrix
		m_ProjectionMatrix = glm::perspective(1.0472f, m_AspectRatio, 0.1f, 100.0f);

		// Build model view matrix with the stack.
		m_ModelViewStack.push(glm::translate(glm::mat4(1.0f), -m_CameraPosition));
		m_ModelViewStack.push(m_ModelViewStack.top());
		m_ModelViewStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		m_ModelViewStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(currentTime * 0.1f, currentTime * 0.4f, currentTime * 0.23f));

		// Copy perspective and MV matrices to corresponding uniform variables.
		m_Shader->SetUniformFloat("uTime", (float)currentTime);
		m_Shader->SetUniformMatrix4("uProjection", m_ProjectionMatrix);
		m_Shader->SetUniformMatrix4("uModel", m_ModelViewStack.top());
		m_Shader->SetUniformMatrix4("uView", m_ModelViewStack.top() * glm::inverse(m_ModelViewStack.top()));

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(1);

		// Associate VBO with the corresponding vertex attribute in the vertex shader.
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

		// Bind program and vertex array object to use while drawing.
		glUseProgram(*m_Shader);
		glBindVertexArray(vao[0]);

		// Adjust OpenGL settings and draw model.
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// Enable wireframe mode.
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		m_ModelViewStack.pop();
		m_ModelViewStack.pop();
	}

	void OpenGLApp::WindowReshapeCallback(GLFWwindow* window, int newWidth, int newHeight)
	{
		//// Build perspective matrix.
		//m_AspectRatio = (float)newWidth / (float)newHeight;
		//glViewport(0, 0, newWidth, newHeight);
		//glm::perspective(1.0472f, m_AspectRatio, 0.1f, 1000.0f); // 1.0472 radians = 60 degree.
	}
}