#pragma once

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Shader.h"
#include "Material.h"

#include <memory>
#include <GLFW/glfw3.h>

namespace OpenGL
{
	class Scene
	{
	public:
		/** Prepare stuff to be rendered in the next steps */
		virtual void BeginScene(GLFWwindow* context) = 0;

		/** Draw skybox, shadow and geometries */
		virtual void RenderScene(GLFWwindow* context, double currentTime) = 0;

		/** Bind the texture skybox and render it */
		virtual void RenderSkyBox(GLFWwindow* context, double currentTime) = 0;

		/** Bind the shadow texture and custom shadow frame buffer and draw on it */
		virtual void RenderShadow(GLFWwindow* context, double currentTime) = 0;

		/** Restore frame buffer, commit uniforms and draw geometry to the screen */
		virtual void RenderGeometry(GLFWwindow* context, double currentTime) = 0;
	};

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
		std::shared_ptr<Camera> m_Camera = NULL;

		// Lights
		std::shared_ptr<Light> m_Light = NULL;

		// Shaders
		std::shared_ptr<Shader> m_DepthTestShader	= NULL;
		std::shared_ptr<Shader> m_ShadowShader		= NULL;
		std::shared_ptr<Shader> m_CubeMapShader		= NULL;

		// Geometry
		std::shared_ptr<Mesh> m_Torus	= NULL;
		std::shared_ptr<Mesh> m_Sphere	= NULL;
		std::shared_ptr<Mesh> m_CubeMap = NULL;

		// FrameBuffers and Textures
		GLuint m_DepthFBO = 0;
		std::shared_ptr<DepthTexture> m_DepthTexture;
		int m_ScreenSizeX = 0;
		int m_ScreenSizeY = 0;
		glm::mat4 m_ShadowTexBias = glm::mat4(1.0f);

		// Materials
		std::shared_ptr<Material> m_BronzeMaterial	= NULL;
		std::shared_ptr<Material> m_SilverMaterial	= NULL;
		std::shared_ptr<Material> m_GoldMaterial	= NULL;
	};
}