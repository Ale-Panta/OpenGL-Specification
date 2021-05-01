#pragma once

#include <GLFW/glfw3.h>

namespace OpenGL
{
	/**
	 * The scene asset works as container of all geometries. It's job is to declare how those geometries will be rendered.
	 * You can't instantiate this class directly, you need to create a child of this one.
	 * The child class will have reference to light, frame buffer, geometries and all things you need to have in order
	 * to renderer the scene you want.
	 */
	class IScene
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
}