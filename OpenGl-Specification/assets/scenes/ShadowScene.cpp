#include "ShadowScene.h"

#include "../../assets/meshes/Cube.h"
#include "../../assets/meshes/Torus.h"
#include "../../assets/meshes/Sphere.h"

#include <glm/ext/matrix_float4x4.hpp>

using namespace std;
using namespace glm;

namespace OpenGL
{
	ShadowScene::ShadowScene()
	{
		// Create camera
		mCamera = make_shared<Camera>(glm::vec3(0.0f, 0.0f, 4.0f), 0.005f, false);

		// Create light
		m_Light = make_shared<Light>();

		// Create Bias matrix (division and addition).
		m_ShadowTexBias = {
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f
		};
	}

	ShadowScene::~ShadowScene()
	{
		glDeleteFramebuffers(1, &m_DepthFBO);
	}

	void ShadowScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		mCamera->ProcessInput(context);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Below are rendered the elements in order. This order is a must and it should be never broken.
		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void ShadowScene::BeginScene(GLFWwindow* context)
	{
		// Create shaders
		m_DepthTestShader = make_shared<Shader>("assets/shaders/Shadow/vertDepthShader.glsl", "assets/shaders/Shadow/fragDepthShader.glsl");
		m_ShadowShader = make_shared<Shader>("assets/shaders/Shadow/vertShadowShader.glsl", "assets/shaders/Shadow/fragShadowShader.glsl");
		mCubeMapShader = make_shared<Shader>("assets/shaders/CubeMap/vertCubeMapShader.glsl", "assets/shaders/CubeMap/fragCubeMapShader.glsl");

		// Create geometries
		m_Torus = make_shared<Torus>();
		mSphere = make_shared<Sphere>();
		mCubeMap = make_shared<Cube>();

		glfwGetFramebufferSize(context, &m_ScreenSizeX, &m_ScreenSizeY);

		// Create custom frame buffer for the shadow.
		glGenFramebuffers(1, &m_DepthFBO);

		// Create the shadow texture and configure it to hold depth information.
		m_DepthTexture = make_shared<TextureShadow>(m_DepthFBO, m_ScreenSizeX, m_ScreenSizeY);
	}

	void ShadowScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
		mCubeMapShader->SetUniformMatrix4("uModel", mCamera->GetModelMatrix());
		mCubeMapShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		mCubeMapShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());
		//mCubeMap->Draw(*mCubeMapShader);
	}

	void ShadowScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
		// Make the custom frame buffer current and associate it with the shadow texture.
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *m_DepthTexture, 0);

		// Disable drawing colors but enable the depth computation
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glEnable(GL_DEPTH_TEST);

		// Resolving shadow acne artifact
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(3.0f, 4.0f);

		// #Begin pass one...

		glClear(GL_DEPTH_BUFFER_BIT);

		mat4 torusModel = translate(glm::mat4(1.0f), vec3(0.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_DepthTestShader->SetUniformMatrix4("uShadowMVP", m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * torusModel);
		//m_Torus->Draw(*m_DepthTestShader);

		glCullFace(GL_FRONT);	// Avoid wrong shadow rendering on sphere

		mat4 sphereModel = translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 3.0f, 0.0f, cos((float)currentTime) * 2.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		m_DepthTestShader->SetUniformMatrix4("uShadowMVP", m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * sphereModel);
		//mSphere->Draw(*m_DepthTestShader);

		glCullFace(GL_BACK);	// Reset cull face.

		// #End pass one
	}

	void ShadowScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		glPolygonOffset(0.0f, 0.0f);
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Restore default display buffer, and re-enable drawing.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_DepthTexture);
		glDrawBuffer(GL_FRONT);

		// #Begin pass two...

		glClear(GL_DEPTH_BUFFER_BIT);

		m_Light->CommitToProgram(*m_ShadowShader);

		m_ShadowShader->SetUniformFloat("uTime", (float)currentTime);

		m_ShadowShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		m_ShadowShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());

		mat4 torusModel = translate(glm::mat4(1.0f), vec3(0.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//m_SilverMaterial->CommitToProgram(*m_ShadowShader);
		m_ShadowShader->SetUniformMatrix4("uModel", torusModel);
		m_ShadowShader->SetUniformMatrix4("uShadowMVP", m_ShadowTexBias * m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * torusModel);
		//m_Torus->Draw(*m_ShadowShader);

		mat4 sphereModel = translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 3.0f, 0.0f, cos((float)currentTime) * 2.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		//m_GoldMaterial->CommitToProgram(*m_ShadowShader);
		m_ShadowShader->SetUniformMatrix4("uModel", sphereModel);
		m_ShadowShader->SetUniformMatrix4("uShadowMVP", m_ShadowTexBias * m_Light->GetProjMatrix() * m_Light->GetViewMatrix() * sphereModel);
		//mSphere->Draw(*m_ShadowShader);

		// #End pass two
	}

}