#include "NormalMappingScene.h"
#include "../../src/Material.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"

using namespace std;
using namespace glm;

namespace OpenGL
{
	void NormalMappingScene::BeginScene(GLFWwindow* context)
	{
		m_Camera = make_shared<Camera>(vec3(.0f, .0f, 4.0f), .02f);
		m_NormalMapShader = make_shared<Shader>("assets/shaders/NormalMapping/vertNormalMappingShader.glsl", "assets/shaders/NormalMapping/fragNormalMappingShader.glsl");
		m_LightSource = make_shared<Light>();
		m_BronzeMaterial = make_shared<BronzeMaterial>();
		m_Sphere = make_shared<Sphere>();
		m_NormalMap = make_shared<Texture2D>("assets/textures/normalmap_brick.png");
	}

	void NormalMappingScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		m_Camera->ProcessInput(context);

		glClearColor(.0f, .0f, .1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void NormalMappingScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void NormalMappingScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void NormalMappingScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mat4 sphereModelMatrix = translate(mat4(1.0f), vec3(.0f));
		
		m_NormalMapShader->SetUniformMatrix4("uModel", sphereModelMatrix);
		m_NormalMapShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_NormalMapShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());

		m_LightSource->CommitToProgram(*m_NormalMapShader);
		m_BronzeMaterial->CommitToProgram(*m_NormalMapShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_NormalMap);
		m_NormalMapShader->SetUniformInt("uNormalMap", 0);
		m_Sphere->Draw(*m_NormalMapShader);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}