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
		void DrawShadow();
		void DrawOpaque();
		void DrawTransparents();

	private:
		// Scene stuff
		Light* m_LightSrc = nullptr;
		Camera* m_Camera = nullptr;

		// Env
		Mesh* m_GroundPlane = nullptr;

		// Opaque
		Mesh* m_SmallOpaqueSphere = nullptr;
		Mesh* m_MediumOpaqueSphere = nullptr;
		Mesh* m_LargeOpaqueSphere = nullptr;

		// Transparent
		Mesh* m_SmallTransparentSphere = nullptr;
		Mesh* m_MediumTransparentSphere = nullptr;
		Mesh* m_LargeTransparentSphere = nullptr;
		Mesh* m_ViewportPlane = nullptr;

		// Shaders
		Shader* m_CommitShadowShader = nullptr;
		Shader* m_ViewShadowShader = nullptr;
		Shader* m_DrawShadowShader = nullptr;

		Shader* m_PBRShader = nullptr;

		Shader* m_CommitTransparentShader = nullptr;
		Shader* m_ResolveTransparentShader = nullptr;

		// Textures
	};

}