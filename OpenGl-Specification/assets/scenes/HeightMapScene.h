#pragma once
#include "../../src/Scene.h"
#include "../../src/Mesh.h"
#include "../../src/Light.h"
#include "../../src/Camera.h"
#include "../../src/Material.h"
#include "../../src/Texture.h"

#include <memory>

namespace OpenGL
{
	class HeightMapScene : public Scene
	{
	public:
		HeightMapScene() = default;

	public:
		void BeginScene(GLFWwindow * context) override;
		void RenderScene(GLFWwindow * context, double currentTime) override;
		void RenderSkyBox(GLFWwindow * context, double currentTime) override;
		void RenderShadow(GLFWwindow * context, double currentTime) override;
		void RenderGeometry(GLFWwindow * context, double currentTime) override;

	private:
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<Shader> m_HeightMapShader;
		std::shared_ptr<Light> m_LightSource;
		std::shared_ptr<Material> m_BronzeMaterial;
		std::shared_ptr<Mesh> m_Sphere;
		std::shared_ptr<Texture2D> m_HeightMap;
	};
}