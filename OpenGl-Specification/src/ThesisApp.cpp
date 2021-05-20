#include "ThesisApp.h"

namespace OpenGL
{

	ThesisApp::ThesisApp(const char* name, int width, int height, int samples)
		: OpenGLApp(name, width, height, samples)
	{
	}

	ThesisApp::~ThesisApp()
	{
		delete m_LightSrc;
		delete m_Camera;

		delete m_GroundPlane;

		delete m_SmallOpaqueSphere;
		delete m_MediumOpaqueSphere;
		delete m_LargeOpaqueSphere;

		delete m_SmallTransparentSphere;
		delete m_MediumTransparentSphere;
		delete m_LargeTransparentSphere;
		delete m_ViewportPlane;

		delete m_CommitShadowShader;
		delete m_ViewShadowShader;
		delete m_DrawShadowShader;

		delete m_PBRShader;

		delete m_CommitTransparentShader;
		delete m_ResolveTransparentShader;
	}

	bool ThesisApp::Initialize()
	{
		OpenGLApp::Initialize();

		// Initialize resources...

		m_PBRShader = new Shader("assets/shaders/PBR/vertCookTorranceShader.glsl", "assets/shaders/PBR/fragCookTorranceShader.glsl");

		m_CommitTransparentShader = new Shader("assets/shaders/OIT/vertBuildListShader.glsl", "assets/shaders/OIT/fragBuildListShader.glsl");
		m_ResolveTransparentShader = new Shader("assets/shaders/OIT/vertResolveListShader.glsl", "assets/shaders/OIT/fragResolveListShader.glsl");

		CreateOITBuffers();
		CreateDepthBuffer();

		return true;
	}

	void ThesisApp::Update(double gt)
	{
		RefresOITBuffers();

		// Update light and camera position.
	}

	void ThesisApp::Draw(double gt)
	{
		glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);
	}

	void ThesisApp::DrawShadow()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);
		glPolygonOffset(2.0f, 4.0f);

		// use proper program
		// Draw geometries...

		glDisable(GL_POLYGON_OFFSET_FILL);

		// Restore the default frame buffer and field of view
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_ClientWidth, m_ClientHeight);

		// glUseProgram(*m_ShadowResult);
		// glBindTexture(GL_TEXTURE_2D, *m_DepthTexture);
		// m_Plane->Draw(*m_ShadowResult);

		// Now render from the viewer's position
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void ThesisApp::DrawOpaque()
	{
		glDepthMask(GL_TRUE);

		// use proper program
		// Draw geometries...
	}

	void ThesisApp::DrawTransparents()
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// use proper program
		// Draw geometries...

		// use proper program
		// Render it to a plane.
	}

}