#pragma once
#include "OpenGLApp.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

namespace OpenGL
{
	class ThesisApp : public OpenGLApp
	{
	public:
		ThesisApp(const char* name, int width, int height, int samples);
		~ThesisApp() override;

	public:
		bool Initialize() override;

	protected:
		/**
		 * Application update loop.
		 * Refresh all buffers related to OIT and update camera and light uniform object buffer.
		 * @see RefreshOITBuffers.
		 */
		void Update(double gt) override;

		/**
		 * Call the draw application stages.
		 * Clear color and depth buffer bit.
		 * Activate the depth mask and disable blending.
		 * Now they are called the draw stages in order.
		 * @see DrawShadow.
		 * @see	DrawOpaque.
		 * @see DrawTransparents.
		 */
		void Draw(double gt) override;

		/**
		 * Draw shadow stage.
		 * Enable GL_DEPTH_TEST and GL_CULL_FACE.
		 * Bind the shadow frame buffer object.
		 * Set the viewport to the shadow texture size.
		 * Set the clear depth to 1.0f and clear depth buffer bit.
		 * Enable polygon offset and set it.
		 * Commit shadow to texture using the proper shader program.
		 * Reset values disabling polygon offset, restore the frame buffer object and viewport size.
		 * Now you can render the shadow texture to the plane in viewport.
		 * @see m_ShadowTexture.
		 */
		void DrawShadow(double gt);

		/**
		 * Draw opaque stage.
		 * Clear color and depth buffer bit.
		 * Draw geometries with the proper shader program.
		 */
		void DrawOpaque(double gt);

		/**
		 * Draw transparent stage.
		 * Disable cull face and depth mask.
		 * Enable blending.
		 * Draw transparent geometries with the proper shader program.
		 * Now draw the resolved alpha values to plane mesh in viewport space.
		 */
		void DrawTransparents(double gt);

	private:

		/**
		 * Scene light source.
		 * @see Camera.
		 */
		Light* m_LightSrc = nullptr;

		/**
		 * Scene camera.
		 * @see Camera.
		 */
		Camera* m_Camera = nullptr;


		// Utils mesh

		/**
		 * Plane mesh.
		 * Used to show the shadow map and transparent results in the viewport.
		 */
		Mesh* m_ViewportPlane = nullptr;

		// Environment
		Mesh* m_GroundPlane = nullptr;

		Mesh* m_SmallOpaqueSphere		= nullptr;
		Mesh* m_MediumOpaqueSphere		= nullptr;
		Mesh* m_LargeOpaqueSphere		= nullptr;
		Mesh* m_SmallOpaqueSphereTwo	= nullptr;
		Mesh* m_MediumOpaqueSphereTwo	= nullptr;

		// Transparent
		Mesh* m_SmallTransparentSphere		= nullptr;
		Mesh* m_MediumTransparentSphere		= nullptr;
		Mesh* m_LargeTransparentSphere		= nullptr;
		Mesh* m_SmallTransparentSphereTwo	= nullptr;
		Mesh* m_MediumTransparentSphereTwo	= nullptr;

		/**
		 * Shader.
		 * This shader is used to upload properly all uniform blocks in common among all shaders.
		 */
		Shader* m_UBOSettingShader = nullptr;

		/**
		 * Shader.
		 * This shader render the geometries from the light's point of view.
		 * It just draw the depth into the proper buffer, not the default one.
		 */
		Shader* m_CommitShadowShader = nullptr;

		/**
		 * Shader.
		 * Allow us to see what the shadow texture look like.
		 * It render the geometry in viewport space and sample the texture.
		 */
		Shader* m_ViewShadowShader = nullptr;

		/**
		 * Shader.
		 * Physically based rendering shader. It calculate also the shadows.
		 */
		Shader* m_PBRShader = nullptr;

		/**
		 * Shader.
		 * Commit the pixel color into image buffer.
		 * @warning	Use this program only to render geometries that you consider transparent. You can eventually sort
		 * all geometries and put in the transparent stage the ones with alpha less than one.
		 * @see DrawTransparents.
		 */
		Shader* m_CommitTransparentShader = nullptr;

		/**
		 * Shader.
		 * Used at the end of the transparent stage to sort the alpha values committed at the beginning of the same stage.
		 * @see DrawTransparents.
		 */
		Shader* m_ResolveTransparentShader = nullptr;

		/**
		 * Camera UBO.
		 * Uniform buffer object where to upload all camera's data.
		 * @see CameraProperties.
		 */
		GLuint m_UBOCameraPrties = 0;

		/**
		 * Light UBO.
		 * Uniform buffer object where to upload all light's data.
		 * @see LightProperties.
		 */
		GLuint m_UBOLightPrties = 0;

		// Pirate gold textures
		Texture* m_PG_Albedo	= nullptr;
		Texture* m_PG_AO		= nullptr;
		Texture* m_PG_Height	= nullptr;
		Texture* m_PG_Metallic	= nullptr;
		Texture* m_PG_Normal	= nullptr;
		Texture* m_PG_Roughness = nullptr;

		// Cavern deposit textures
		Texture* m_CD_Albedo	= nullptr;
		Texture* m_CD_AO		= nullptr;
		Texture* m_CD_Height	= nullptr;
		Texture* m_CD_Metallic	= nullptr;
		Texture* m_CD_Normal	= nullptr;
		Texture* m_CD_Roughness = nullptr;

		// Stone block wall textures
		Texture* m_SBW_Albedo	= nullptr;
		Texture* m_SBW_AO		= nullptr;
		Texture* m_SBW_Height	= nullptr;
		Texture* m_SBW_Metallic	= nullptr;
		Texture* m_SBW_Normal	= nullptr;
		Texture* m_SBW_Roughness= nullptr;

		// Grey granite textures
		Texture* m_GG_Albedo	= nullptr;
		Texture* m_GG_AO		= nullptr;
		Texture* m_GG_Metallic	= nullptr;
		Texture* m_GG_Normal	= nullptr;
		Texture* m_GG_Roughness = nullptr;
	};
}