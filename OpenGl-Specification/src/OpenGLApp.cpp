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
		m_Camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 4.0f), 0.005f, false);

		// Create geometries
		m_Torus		= std::make_shared<Torus>();
		m_Sphere	= std::make_shared<Sphere>();

		// Create light source
		m_Light	= std::make_shared<Light>();

		// Create materials
		m_SilverMaterial	= std::make_shared<SilverMaterial>();
		m_GoldMaterial		= std::make_shared<GoldMaterial>();

		// Create shaders
		m_LitShader			= std::make_shared<Shader>("assets/shaders/BlinnPhong/vertBlinnPhongShader.glsl", "assets/shaders/BlinnPhong/fragBlinnPhongShader.glsl");
		m_DepthTestShader	= std::make_shared<Shader>("assets/shaders/Shadow/vertDepthShader.glsl", "assets/shaders/Shadow/fragDepthShader.glsl");
		m_ShadowShader		= std::make_shared<Shader>("assets/shaders/Shadow/vertShadowShader.glsl", "assets/shaders/Shadow/fragShadowShader.glsl");

		glfwGetFramebufferSize(window, &m_Width, &m_Height);
		m_ScreenSizeX = m_Width;
		m_ScreenSizeY = m_Height;

		// Create custom frame buffer
		glGenFramebuffers(1, &m_DepthFBO);

		// Create the shadow texture and configure it to hold depth information.
		glGenTextures(1, &m_DepthTex);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);
		glBindTexture(GL_TEXTURE_2D, m_DepthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_ScreenSizeX, m_ScreenSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_TextureTransormation = {
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f
		};
	}


	void OpenGLApp::Display(GLFWwindow* context, double currentTime)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Make the custom frame buffer current and associate it with the shadow texture.
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTex, 0);

		// Disable drawing colors but enable the depth computation
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 16.0f);

		// #Begin pass one...

		glClear(GL_DEPTH_BUFFER_BIT);

		mat4 torusModel = translate(glm::mat4(1.0f), vec3(0.0f)) *glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_DepthTestShader->SetUniformMatrix4("uShadowMVP", m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * torusModel);
		m_Torus->Draw(*m_DepthTestShader);

		mat4 sphereModel = translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 3.0f, 0.0f, cos((float)currentTime) * 2.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_DepthTestShader->SetUniformMatrix4("uShadowMVP", m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * sphereModel);
		m_Sphere->Draw(*m_DepthTestShader);

		// #End pass one

		glPolygonOffset(0.0f, 0.0f);
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Restore default display buffer, and re-enable drawing.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DepthTex);
		glDrawBuffer(GL_FRONT);

		// #Begin pass two...

		glClear(GL_DEPTH_BUFFER_BIT);

		m_Light->CommitToShader(*m_ShadowShader);

		m_ShadowShader->SetUniformFloat("uTime", (float)currentTime);

		m_ShadowShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_ShadowShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());

		m_SilverMaterial->CommitToProgram(*m_ShadowShader);
		m_ShadowShader->SetUniformMatrix4("uModel", torusModel);
		m_ShadowShader->SetUniformMatrix4("uShadowMVP", m_TextureTransormation * m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * torusModel);
		m_Torus->Draw(*m_ShadowShader);

		m_GoldMaterial->CommitToProgram(*m_ShadowShader);
		m_ShadowShader->SetUniformMatrix4("uModel", sphereModel);
		m_ShadowShader->SetUniformMatrix4("uShadowMVP", m_TextureTransormation * m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * sphereModel);
		m_Sphere->Draw(*m_ShadowShader);

		// #End pass two
	}
}