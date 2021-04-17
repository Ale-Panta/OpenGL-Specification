#pragma once
#include "../../src/Scene.h"
#include "../../src/Camera.h"
#include "../../src/Shader.h"
#include "../../src/Material.h"
#include "../../src/Mesh.h"
#include <stack>

namespace OpenGL
{
	class PBRScene : public Scene
	{

	public:
		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		std::shared_ptr<Camera>		mCamera;
		std::shared_ptr<glm::vec3>	mLightPosition;
		std::shared_ptr<glm::vec3>	mLightColor;
		
		std::stack<glm::mat4> mModelMatStack;
		
		// Cubemap
		std::shared_ptr<Mesh>			mCubeMap;
		std::shared_ptr<Shader>			mCubeMapShader;
		std::shared_ptr<TextureCubeMap> mTexCubeMap;

		// Geometry scene
		std::shared_ptr<Mesh>		mSphere;
		std::shared_ptr<Shader>		mPBRShader;
		std::shared_ptr<Material>	mMaterial;
	};
}