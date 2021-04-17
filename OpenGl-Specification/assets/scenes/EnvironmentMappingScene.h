#pragma once

#include "../../src/Texture.h"
#include "../../src/Shader.h"
#include "../../src/Scene.h"
#include "../../src/Mesh.h"

#include <memory>

namespace OpenGL
{
	class EnvironmentMappingScene : public Scene
	{
	public:
		EnvironmentMappingScene() = default;


	public:
		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		std::shared_ptr<Camera> mCamera;

		std::shared_ptr<Material>	m_EnvMappingMaterial;
		std::shared_ptr<Shader>		m_EnvMappingShader;
		std::shared_ptr<Shader>		mCubeMapShader;

		std::shared_ptr<Mesh> mSphere;

		std::shared_ptr<Mesh> mCubeMap;

		std::shared_ptr<TextureCubeMap> mTexCubeMap;
	};
}