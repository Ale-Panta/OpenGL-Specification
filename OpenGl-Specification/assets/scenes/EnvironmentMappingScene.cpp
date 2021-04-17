#include "EnvironmentMappingScene.h"

#include "../meshes/Sphere.h"
#include "../meshes/Cube.h"

using namespace std;
using namespace glm;

namespace OpenGL
{

	void EnvironmentMappingScene::BeginScene(GLFWwindow* context)
	{
		mCamera = make_shared<Camera>(vec3(0.0f, 0.0f, 4.0f), 0.03f);
		mSphere = make_shared<Sphere>();
		mCubeMap = make_shared<Cube>();
		mTexCubeMap = make_shared<TextureCubeMap>(
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
			);
		mCubeMapShader = make_shared<Shader>("assets/shaders/CubeMap/vertCubeMapShader.glsl", "assets/shaders/CubeMap/fragCubeMapShader.glsl");
		m_EnvMappingShader = make_shared<Shader>("assets/shaders/EnvironmentMapping/vertEnvMapping.glsl", "assets/shaders/EnvironmentMapping/fragEnvMapping.glsl");
	}

	void EnvironmentMappingScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		mCamera->ProcessInput(context);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void EnvironmentMappingScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
		//mCubeMapShader->SetUniformMatrix4("uModel", mCamera->GetModelMatrix());
		//mCubeMapShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		//mCubeMapShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());
		//mCubeMap->Draw(*mCubeMapShader);
	}

	void EnvironmentMappingScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void EnvironmentMappingScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mat4 sphereModel = translate(mat4(1.0f), vec3(0.0f));

		m_EnvMappingShader->SetUniformMatrix4("uModel", sphereModel);
		m_EnvMappingShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		m_EnvMappingShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, *mTexCubeMap);
		//mSphere->Draw(*m_EnvMappingShader);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}