#pragma once

#include "../../src/Shader.h"
#include "../../src/Mesh.h"
#include "../../src/Light.h"
#include "../../src/Texture.h"
#include "../../src/Camera.h"
#include "../../src/Scene.h"
#include <stack>
#include <memory>

namespace OpenGL
{
	/** This is the max supported width. We could support higher resolutions but this is reasonable for this application */
	static constexpr int MAX_TEX_WIDTH = 2048;
	/** This is the max supported height. We could support higher resolutions but this is reasonable for this application */
	static constexpr int MAX_TEX_HEIGHT = 2048;

	class OITScene : public IScene
	{
	public:
		/**
		 * LitScene constructor.
		 * @param fovy - vertical field of view
		 * @param aspecRatio - window aspectRatio
		 * @param near - camera near plane
		 * @param far - camera far plane
		 */
		OITScene(float fovy, float aspectRatio, float near, float far);

		/**
		 * LitScene destructor.
		 * Delete every resource you declare.
		 * Actually there is no need because they are cleared on program close but it is a good practice.
		 */
		~OITScene();

		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderOpaque(GLFWwindow* context, double currentTime) override;
		void RenderTrasparency(GLFWwindow* context, double currentTime) override;

	private:
		/**
		 * Camera object.
		 * Represents our view point.
		 * @see Camera.
		 */
		Camera* m_Camera		= NULL;

		/**
		 * Light object.
		 * Represent our source light.
		 * @see Light.
		 */
		Light*	m_LightSource	= NULL;
		
		/**
		 * PBR Shader.
		 * PBR rendering using metallic work flow.
		 * @see Shader.
		 */
		Shader* m_LitShader		= NULL;

		/**
		 * Unlit Shader.
		 * It's utilized to render the source light.
		 * @see Shader.
		 */
		Shader* m_UnlitShader		= NULL;

		/**
		 * OIT list builder Shader.
		 * It is attached to every transparent object to be rendered.
		 * This is done in the first stage of rendering transparent.
		 * @see Shader.
		 */
		Shader* m_BuildListShader = NULL;

		/**
		 * OIT list resolver Shader.
		 * It is attached to a Plane Mesh attached to viewport.
		 * This program reorder and render the pixels build in the previous stage.
		 * This is done in the second stage of rendering transparent.
		 * @see Shader.
		 * @see Mesh.
		 */
		Shader* m_ResolveListShader = NULL;

		/** Cavern deposit mesh */
		Mesh* m_SphereCD	= NULL;
		/** Pirate gold mesh */
		Mesh* m_SpherePG	= NULL;
		/** Space ship mesh */
		Mesh* m_SphereSS	= NULL;
		/** Vented metal mesh */
		Mesh* m_SphereVM	= NULL;

		/** Plane to render transparent geometries on the screen */
		Mesh* m_Plane = NULL;

		/** Light properties uniform object buffer  */
		GLuint m_UBOLightPrties = 0;

		/** Camera properties uniform object buffer  */
		GLuint m_UBOCamPrties = 0;

		/** 2D image that will be used to store the head pointers for the per-pixel linked lists */
		GLuint head_pointer_texture = 0;

		/** Pixel Buffer Object for clearing the 2D image pixels store */
		GLuint head_pointer_clear_buffer = 0;

		/** Atomic counter buffer to back our atomic counter */
		GLuint atomic_counter_buffer = 0;

		GLuint* data = NULL;
		GLuint linked_list_buffer = 0;
		GLuint linked_list_texture = 0;
	};
}