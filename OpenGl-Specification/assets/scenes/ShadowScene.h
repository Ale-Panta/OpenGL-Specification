#pragma once

#include "../../src/Scene.h"

#include <memory>

namespace OpenGL
{
	class ShadowScene : Scene
	{
	public:
		ShadowScene();
		~ShadowScene();

	public:
		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	public:
		// Rendering
		std::shared_ptr<Camera> mCamera = NULL;

		// Lights
		std::shared_ptr<Light> m_Light = NULL;

		// Shaders
		std::shared_ptr<Shader> m_DepthTestShader = NULL;
		std::shared_ptr<Shader> m_ShadowShader = NULL;
		std::shared_ptr<Shader> mCubeMapShader = NULL;

		// Geometry
		std::shared_ptr<Mesh> m_Torus = NULL;
		std::shared_ptr<Mesh> mSphere = NULL;
		std::shared_ptr<Mesh> mCubeMap = NULL;

		// FrameBuffers and Textures
		GLuint m_DepthFBO = 0;
		std::shared_ptr<TextureShadow> m_DepthTexture;
		int m_ScreenSizeX = 0;
		int m_ScreenSizeY = 0;

		/** Matrix transformation allow us to transform the coordinates from (-1, 1) to (0, 1) */
		glm::mat4 m_ShadowTexBias = glm::mat4(1.0f);

		// Materials
		std::shared_ptr<Material> m_BronzeMaterial = NULL;
		std::shared_ptr<Material> m_SilverMaterial = NULL;
		std::shared_ptr<Material> m_GoldMaterial = NULL;
	};
}