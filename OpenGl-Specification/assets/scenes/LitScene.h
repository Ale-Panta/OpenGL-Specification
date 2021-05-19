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
	class LitScene : public IScene
	{
	public:
		/**
		 * LitScene constructor
		 * 
		 * @param fovy, vertical field of view
		 * @param aspecRatio, window aspectRatio
		 * @param near, camera near plane
		 * @param far, camera far plane
		 */
		LitScene(float fovy, float aspectRatio, float near, float far);

		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		Camera* m_Camera		= NULL;
		Light*	m_LightSource	= NULL;
		
		Shader* m_ShadowShader		= NULL;
		Shader* m_SceneShader		= NULL;
		Shader* m_ShadowResult		= NULL;
		Shader* m_LitShader		= NULL;

		/** Cavern deposit mesh */
		Mesh* m_SphereCD	= NULL;
		/** Pirate gold mesh */
		Mesh* m_SpherePG	= NULL;
		/** Space ship mesh */
		Mesh* m_SphereSS	= NULL;
		/** Vented metal mesh */
		Mesh* m_SphereVM	= NULL;

		Mesh* m_Plane		= NULL;
		Mesh* m_Terrain		= NULL;

		// --- Textures ---
 
		// Begin Cavern Deposit
		Texture2D* m_CD_Albedo		= NULL;
		Texture2D* m_CD_AO			= NULL;
		Texture2D* m_CD_Height		= NULL;
		Texture2D* m_CD_Metallic	= NULL;
		Texture2D* m_CD_Normal		= NULL;
		Texture2D* m_CD_Roughness	= NULL;
		// End Cavern Deposit

		// Begin Pirate Gold
		Texture2D* m_PG_Albedo		= NULL;
		Texture2D* m_PG_AO			= NULL;
		Texture2D* m_PG_Height		= NULL;
		Texture2D* m_PG_Metallic	= NULL;
		Texture2D* m_PG_Normal		= NULL;
		Texture2D* m_PG_Roughness	= NULL;
		// End Pirate Gold

		// Begin Space Ship
		Texture2D* m_SS_Albedo		= NULL;
		Texture2D* m_SS_AO			= NULL;
		Texture2D* m_SS_Height		= NULL;
		Texture2D* m_SS_Metallic	= NULL;
		Texture2D* m_SS_Normal		= NULL;
		Texture2D* m_SS_Roughness	= NULL;
		// End Space Ship

		// Begin Vented Metal
		Texture2D* m_VM_Albedo		= NULL;
		Texture2D* m_VM_AO			= NULL;
		Texture2D* m_VM_Height		= NULL;
		Texture2D* m_VM_Metallic	= NULL;
		Texture2D* m_VM_Normal		= NULL;
		Texture2D* m_VM_Roughness	= NULL;
		// End Vented Metal

		/** Light properties uniform object buffer  */
		GLuint m_UBOLightPrties = 0;

		/** Camera properties uniform object buffer  */
		GLuint m_UBOCamPrties = 0;

		// FBO to render depth with
		GLuint m_DepthFBO = 0;
		TextureShadow* m_DepthTexture = NULL;
	};
}