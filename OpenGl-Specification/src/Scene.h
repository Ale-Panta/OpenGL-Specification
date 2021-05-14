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
		/**
		 * Initialize the scene.
		 * Initialize here all resources of the scene.
		 * @warning Shaders, textures, and other assets related to OpenGL must be initialized here and not in the constructor of the scene.
		 * @param context - context window.
		 */
		virtual void BeginScene(GLFWwindow* context) = 0;

		/**
		 * Render the scene.
		 * It clears depth and color buffer.
		 * Calls RenderSkyBox, RenderShadow, RenderOpaque, RenderTransparency, respectively.
		 * @param context - context window.
		 * @param currentTime - application's time elapsed.
		 * @see RenderSkyBox.
		 * @see RenderOpaque.
		 * @see RenderTrasparency.
		 */
		virtual void RenderScene(GLFWwindow* context, double currentTime) = 0;

		/**
		 * Render the sky box.
		 * It must be the first to be rendered.
		 * The cube wrap the camera, you need to enable the back face rendering. The model matrix must be the camera's model.
		 * The cube must be renderer with the proper shader program.
		 * @param context - context window.
		 * @param currentTime - application's time elapsed.
		 */
		virtual void RenderSkyBox(GLFWwindow* context, double currentTime) = 0;

		/** 
		 * Render shadow map.
		 * New dedicated buffer is bound to the buffer. All geometries are rendered with the proper shader program.
		 * The scene is renderer from the Light source point of view this step.
		 * The view and projection matrices uploaded to the shader comes from the Light source.
		 * @param context - context window.
		 * @param currentTime - application's time elapsed.
		 * @see Light.
		 */
		virtual void RenderShadow(GLFWwindow* context, double currentTime) = 0;

		/**
		 * Render the scene.
		 * Restore the frame buffer to the default one.
		 * Here the geometries are rendered with the shader programs that you want.
		 * The scene must be rendered from the camera point of view.
		 * The view and projection matrices uploaded to the shader comes from the Camera.
		 * @param context - context window.
		 * @param currentTime - application's time elapsed.
		 * @see Camera.
		 */
		virtual void RenderOpaque(GLFWwindow* context, double currentTime) = 0;

		/**
		 * Render the scene.
		 * Here the transparent geometries are rendered with the shader programs.
		 * The scene must be rendered from the camera point of view.
		 * The view and projection matrices uploaded to the shader comes from the Camera.
		 * @param context - context window.
		 * @param currentTime - application's time elapsed.
		 * @see Camera.
		 */
		virtual void RenderTrasparency(GLFWwindow* context, double currentTime) = 0;
	};
}