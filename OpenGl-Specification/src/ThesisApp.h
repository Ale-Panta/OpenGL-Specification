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
		void Update(double gt) override;
		void Draw(double gt) override;
		void DrawShadow(double gt);
		void DrawOpaque(double gt);
		void DrawTransparents(double gt);

	private:
		// Scene stuff
		Light* m_LightSrc = nullptr;
		Camera* m_Camera = nullptr;

		// Env
		Mesh* m_GroundPlane = nullptr;

		// Opaque
		Mesh* m_SmallOpaqueSphere = nullptr;
		Mesh* m_SmallOpaqueSphereTwo = nullptr;
		Mesh* m_MediumOpaqueSphere = nullptr;
		Mesh* m_MediumOpaqueSphereTwo = nullptr;
		Mesh* m_LargeOpaqueSphere = nullptr;
		Mesh* m_LargeOpaqueSphereTwo = nullptr;

		// Transparent
		Mesh* m_SmallTransparentSphere = nullptr;
		Mesh* m_SmallTransparentSphereTwo = nullptr;
		Mesh* m_MediumTransparentSphere = nullptr;
		Mesh* m_MediumTransparentSphereTwo = nullptr;
		Mesh* m_LargeTransparentSphere = nullptr;
		Mesh* m_LargeTransparentSphereTwo = nullptr;
		Mesh* m_ViewportPlane = nullptr;

		// Shaders
		Shader* m_UBOSettingShader = nullptr;

		Shader* m_CommitShadowShader = nullptr;
		Shader* m_ViewShadowShader = nullptr;
		Shader* m_DrawShadowShader = nullptr;

		Shader* m_PBRShader = nullptr;

		Shader* m_CommitTransparentShader = nullptr;
		Shader* m_ResolveTransparentShader = nullptr;

		GLuint m_UBOCameraPrties = 0;
		GLuint m_UBOLightPrties = 0;

		// Textures
		Texture* m_PG_Albedo	= nullptr;
		Texture* m_PG_AO		= nullptr;
		Texture* m_PG_Height	= nullptr;
		Texture* m_PG_Metallic	= nullptr;
		Texture* m_PG_Normal	= nullptr;
		Texture* m_PG_Roughness = nullptr;

		Texture* m_CD_Albedo	= nullptr;
		Texture* m_CD_AO		= nullptr;
		Texture* m_CD_Height	= nullptr;
		Texture* m_CD_Metallic	= nullptr;
		Texture* m_CD_Normal	= nullptr;
		Texture* m_CD_Roughness = nullptr;

		Texture* m_SBW_Albedo	= nullptr;
		Texture* m_SBW_AO		= nullptr;
		Texture* m_SBW_Height	= nullptr;
		Texture* m_SBW_Metallic	= nullptr;
		Texture* m_SBW_Normal	= nullptr;
		Texture* m_SBW_Roughness= nullptr;

		Texture* m_GG_Albedo	= nullptr;
		Texture* m_GG_AO		= nullptr;
		Texture* m_GG_Metallic	= nullptr;
		Texture* m_GG_Normal	= nullptr;
		Texture* m_GG_Roughness = nullptr;
	};
}