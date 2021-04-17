#pragma once
#include "../../src/Mesh.h"
#include "../../src/Light.h"
#include "../../src/Scene.h"
#include "../../src/Camera.h"
#include "../../src/Material.h"
#include "../../src/Texture.h"

#include <memory>

namespace OpenGL
{
	class NormalMappingScene : public Scene
	{
	public:
		NormalMappingScene() = default;

	public:
		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		std::shared_ptr<Camera> mCamera;
		std::shared_ptr<Shader> m_NormalMapShader;
		std::shared_ptr<Light> m_LightSource;
		std::shared_ptr<Material> m_BronzeMaterial;
		std::shared_ptr<Mesh> mSphere;
		std::shared_ptr<Texture2D> mNormalMap;
	};
}