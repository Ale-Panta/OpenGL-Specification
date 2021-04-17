#include "HeightMapScene.h"
#include "../../src/Material.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"

using namespace glm;
using namespace std;

namespace OpenGL
{
	void HeightMapScene::BeginScene(GLFWwindow* context)
	{
		mCamera = make_shared<Camera>(vec3(.0f, .0f, 4.0f), .02f);
		m_HeightMapShader = make_shared<Shader>("assets/shaders/HeightMapping/vertHeightMapShader.glsl", "assets/shaders/HeightMapping/fragHeightMapShader.glsl");
		m_LightSource = make_shared<Light>();
		mSphere = make_shared<Sphere>(256);
		m_HeightMap = make_shared<Texture2D>("assets/textures/heightmap_test.png");
	}

	void HeightMapScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		mCamera->ProcessInput(context);

		glClearColor(.0f, .0f, .1f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void HeightMapScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{

	}

	void HeightMapScene::RenderShadow(GLFWwindow* context, double currentTime)
	{

	}

	void HeightMapScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mat4 sphereModelMatrix = translate(mat4(1.0f), vec3(.0f));

		m_HeightMapShader->SetUniformMatrix4("uModel", sphereModelMatrix);
		m_HeightMapShader->SetUniformMatrix4("uView", mCamera->GetViewMatrix());
		m_HeightMapShader->SetUniformMatrix4("uProjection", mCamera->GetProjMatrix());

		m_LightSource->CommitToProgram(*m_HeightMapShader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_HeightMap);
		m_HeightMapShader->SetUniformInt("uHeightMap", 0);
		//mSphere->Draw(*m_HeightMapShader);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}