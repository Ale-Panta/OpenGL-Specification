#include "OpenGLApp.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

namespace OpenGL
{
	OpenGLApp* OpenGLApp::m_App = nullptr;
	OpenGLApp::OpenGLApp(const char* name, int width, int height, int samples)
		: m_WindowName(name)
		, m_ClientWidth(width)
		, m_ClientHeight(height)
		, m_NumOfSamples(samples)
	{
		m_App = this;
	}

	OpenGLApp::~OpenGLApp()
	{
	}

	OpenGLApp* OpenGLApp::GetApp()
	{
		return m_App;
	}

	float OpenGLApp::GetAspectRatio()
	{
		return (float)m_ClientWidth / (float)m_ClientHeight;
	}

	bool OpenGLApp::Initialize()
	{
		if (!InitMainWindow())
		{
			cout << "Failed to initialize Main Window!" << '\n';
			return false;
		}

		if (!InitiOpenGL())
		{
			cout << "Failed to initialize OpenGL!" << '\n';
			return false;
		}

		return true;
	}

	void OpenGLApp::OnResize()
	{
		// Resize projection matrices.

		glViewport(0, 0, m_ClientWidth, m_ClientHeight);
	}

	bool OpenGLApp::InitMainWindow()
	{
		if (!glfwInit())
		{
			return false;
		}

		// Check versions
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		// Declare multi-sample
		glfwWindowHint(GLFW_SAMPLES, m_NumOfSamples);

		// Create the window context.
		// Note: Creating the window doesn't make it current context by default.
		m_Context = glfwCreateWindow(m_ClientWidth, m_ClientHeight, m_WindowName.c_str(), nullptr, nullptr);

		// Mark the window as current context
		glfwMakeContextCurrent(m_Context);

		// Enable multi-samples.
		glEnable(GL_MULTISAMPLE);

		return true;
	}

	bool OpenGLApp::InitiOpenGL()
	{
		if (glewInit() != GLEW_OK)
		{
			return false;
		}

		// Active swap buffer
		glfwSwapInterval(1);

		// Set up frame buffer
		glfwGetFramebufferSize(m_Context, &m_ClientWidth, &m_ClientHeight);

		return true;
	}


	void OpenGLApp::CreateDepthBuffer()
	{
		glGenTextures(1, &m_ShadowTexture);
		glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_ShadowTexWidth, m_ShadowTexHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Create FBO to render depth into
		glGenFramebuffers(1, &m_ShadowFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_ShadowTexture, 0);
		glDrawBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);	// Restore frame buffer.
	}

	void OpenGLApp::CreateOITBuffers()
	{
		const GLint max_tex_width = 2048;
		const GLint max_tex_height = 2048;

		// Create head pointer texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_HeadPtrTexture);
		glBindTexture(GL_TEXTURE_2D, m_HeadPtrTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, max_tex_width, max_tex_height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindImageTexture(0, m_HeadPtrTexture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

		// Create buffer for clearing the head pointer texture
		glGenBuffers(1, &m_HeadPtrClearBuffer);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_HeadPtrClearBuffer);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, max_tex_width * max_tex_height * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		m_data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		memset(m_data, 0x00, max_tex_width * max_tex_height * sizeof(GLuint));
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

		// Create the atomic counter buffer
		glGenBuffers(1, &m_AtomicCounterBuffer);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_AtomicCounterBuffer);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

		// Create the linked list storage buffer
		glGenBuffers(1, &m_LinkedListBuffer);
		glBindBuffer(GL_TEXTURE_BUFFER, m_LinkedListBuffer);
		glBufferData(GL_TEXTURE_BUFFER, max_tex_width * max_tex_height * 3 * sizeof(vec4), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		// Bind it to a texture (for use as a TBO)
		glGenTextures(1, &m_LinkedListTexture);
		glBindTexture(GL_TEXTURE_BUFFER, m_LinkedListTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, m_LinkedListBuffer);
		glBindTexture(GL_TEXTURE_BUFFER, 0);

		glBindImageTexture(1, m_LinkedListTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glClearDepth(1.0f);	// #TODO: move this line from here.
	}


	void OpenGLApp::RefresOITBuffers()
	{
		// Reset atomic counter
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, m_AtomicCounterBuffer);
		m_data = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);		
		m_data[0] = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		// Clear head-pointer image
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_HeadPtrClearBuffer);
		glBindTexture(GL_TEXTURE_2D, m_HeadPtrTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_ClientWidth, m_ClientHeight, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Bind head-pointer image for read-write
		glBindImageTexture(0, m_HeadPtrTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

		// Bind linked-list buffer for write
		glBindImageTexture(1, m_LinkedListBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);
	}

	void OpenGLApp::Run()
	{
		while (!glfwWindowShouldClose(m_Context))
		{
			glfwSwapBuffers(m_Context);
			glfwPollEvents();					// Handle window-related events such as inputs
			Update(glfwGetTime());
			Draw(glfwGetTime());
		}
		
		// Close OpenGL properly
		glfwDestroyWindow(m_Context);
		glfwTerminate();
	}
}