#include "EnvironmentMappingScene.h"

#include "../meshes/Sphere.h"
#include "../meshes/Cube.h"

using namespace std;
using namespace glm;

namespace OpenGL
{

	void EnvironmentMappingScene::BeginScene(GLFWwindow* context)
	{
		m_Camera = make_shared<Camera>(vec3(0.0f, 0.0f, 4.0f), 0.03f);
		m_Sphere = make_shared<Sphere>();
		m_CubeMap = make_shared<Cube>();
		m_TexCubeMap = make_shared<TextureCubeMap>(
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
			);
		m_CubeMapShader = make_shared<Shader>("assets/shaders/CubeMap/vertCubeMapShader.glsl", "assets/shaders/CubeMap/fragCubeMapShader.glsl");
		m_EnvMappingShader = make_shared<Shader>("assets/shaders/EnvironmentMapping/vertEnvMapping.glsl", "assets/shaders/EnvironmentMapping/fragEnvMapping.glsl");
	}

	void EnvironmentMappingScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		m_Camera->ProcessInput(context);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void EnvironmentMappingScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
		m_CubeMapShader->SetUniformMatrix4("uModel", m_Camera->GetModelMatrix());
		m_CubeMapShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_CubeMapShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());
		m_CubeMap->Draw(*m_CubeMapShader);
	}

	void EnvironmentMappingScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void EnvironmentMappingScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mat4 sphereModel = translate(mat4(1.0f), vec3(0.0f));

		m_EnvMappingShader->SetUniformMatrix4("uModel", sphereModel);
		m_EnvMappingShader->SetUniformMatrix4("uView", m_Camera->GetViewMatrix());
		m_EnvMappingShader->SetUniformMatrix4("uProjection", m_Camera->GetProjMatrix());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, *m_TexCubeMap);
		m_Sphere->Draw(*m_EnvMappingShader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}