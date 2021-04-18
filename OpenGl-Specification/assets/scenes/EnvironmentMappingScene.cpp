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

		mCubeMapMaterial = make_shared<Material>("assets/shaders/CubeMap/vertCubeMapShader.glsl", "assets/shaders/CubeMap/fragCubeMapShader.glsl");
		
		mTexCubeMap = make_shared<TextureCubeMap>(
			"assets/textures/skybox/right.jpg",
			"assets/textures/skybox/left.jpg",
			"assets/textures/skybox/top.jpg",
			"assets/textures/skybox/bottom.jpg",
			"assets/textures/skybox/front.jpg",
			"assets/textures/skybox/back.jpg"
			);

		mCubeMapMaterial->AddTextureParam(make_shared<TextureMatParam>("uCubeMap", mTexCubeMap));
		mCubeMapMaterial->AddMat4PtrParam(std::make_shared<Mat4PtrMatParam>("uView", mCamera->GetViewMatrixPtr()));
		mCubeMapMaterial->AddMat4PtrParam(std::make_shared<Mat4PtrMatParam>("uProjection", mCamera->GetProjMatrixPtr()));
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
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		glDisable(GL_DEPTH_TEST);
		mCubeMap->Draw(*mCubeMapMaterial);
		glEnable(GL_DEPTH_TEST);
	}

	void EnvironmentMappingScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void EnvironmentMappingScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		//mat4 sphereModel = translate(mat4(1.0f), vec3(0.0f));

		//m_EnvMappingShader->SetUniformMatrix4("uModel", sphereModel);
		//m_EnvMappingShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		//m_EnvMappingShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());
		//
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, *mTexCubeMap);
		////mSphere->Draw(*m_EnvMappingShader);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}