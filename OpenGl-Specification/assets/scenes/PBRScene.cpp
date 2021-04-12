#include "PBRScene.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"

using namespace glm;
using namespace std;

namespace OpenGL
{

	void PBRScene::BeginScene(GLFWwindow* context)
	{
		m_Camera = make_shared<Camera>(vec3(.0f, .0f, 4.0f), .01f);
		m_PBRShader = make_shared<Shader>("assets/shaders/PBRCookTorrance/vertCookTorranceShader.glsl", "assets/shaders/PBRCookTorrance/fragCookTorranceShader.glsl");
		m_LightPosition = make_shared<vec3>(2.0f, 2.0f, 5.0f);
		m_LightColor = make_shared<vec3>(1.0f);
		m_BronzeMaterial = make_shared<BronzeMaterial>();
		m_Sphere = make_shared<Sphere>(64);

		m_NormalMap = make_shared<Texture2D>("assets/textures/rustediron/rustediron2_normal.png");
		m_AlbedoMap = make_shared<Texture2D>("assets/textures/rustediron/rustediron2_basecolor.png");
		m_MetallicMap = make_shared<Texture2D>("assets/textures/rustediron/rustediron2_metallic.png");
		m_RoughnessMap = make_shared<Texture2D>("assets/textures/rustediron/rustediron2_roughness.png");
	}

	void PBRScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		m_Camera->ProcessInput(context);

		glClearColor(.05f, .05f, .05f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void PBRScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		m_ModelMatStack.push(translate(mat4(1.0f), vec3(0.0f)));

		m_PBRShader->SetUniformMatrix4("uModel", m_ModelMatStack.top());
		m_PBRShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_PBRShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());

		m_PBRShader->SetUniformVec3("uLight.position", vec3(sin(currentTime) * 10.0f, 0.0f, cos(currentTime) * 10.0f));
		m_PBRShader->SetUniformVec3("uLight.color", vec3(50.0f));

		m_PBRShader->SetUniformVec3("uCameraPosition", m_Camera->GetPosition());
		m_PBRShader->SetUniformFloat("uMaterial.ao", 1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_AlbedoMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *m_MetallicMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, *m_RoughnessMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, *m_NormalMap);

		m_Sphere->Draw(*m_PBRShader);

		m_ModelMatStack.pop();
	}

}