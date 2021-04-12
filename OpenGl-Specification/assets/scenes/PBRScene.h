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
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<glm::vec3> m_LightPosition;
		std::shared_ptr<glm::vec3> m_LightColor;
		
		std::shared_ptr<Shader> m_PBRShader;
		std::shared_ptr<Material> m_BronzeMaterial;

		std::shared_ptr<Mesh> m_Sphere;
		std::stack<glm::mat4> m_ModelMatStack;

		std::shared_ptr<Texture2D> m_AlbedoMap;
		std::shared_ptr<Texture2D> m_MetallicMap;
		std::shared_ptr<Texture2D> m_RoughnessMap;
		std::shared_ptr<Texture2D> m_NormalMap;
		std::shared_ptr<Texture2D> m_AOMap;
	};
}