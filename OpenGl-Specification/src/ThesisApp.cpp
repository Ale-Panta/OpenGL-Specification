#include "ThesisApp.h"
#include "../assets/meshes/Plane.h"
#include "../assets/meshes/Sphere.h"

using namespace std;
using namespace glm;

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

		m_LightSrc = new Light(vec4(0.0f, 5.0f, 0.2f, 0.0f), vec4(0.0f), vec4(50.0f, 45.0f, 43.0f, 1.0f), vec4(1.0f));
		m_Camera = new Camera(vec4(-2.0f, 0.0f, 5.0f, 0.0f), 1.0472f, GetAspectRatio(), 0.1f, 100.0f);

		m_GroundPlane = new Plane();

		m_SmallOpaqueSphere		= new Sphere(256);
		m_MediumOpaqueSphere	= new Sphere(256);
		m_LargeOpaqueSphere		= new Sphere(256);

		m_SmallTransparentSphere = new Sphere(64);
		m_MediumTransparentSphere = new Sphere(64);
		m_LargeTransparentSphere = new Sphere(64);
		m_ViewportPlane = new Plane();

		m_UBOSettingShader = new Shader("assets/shaders/vertUniformBlockSettingsShader.glsl", "assets/shaders/fragUniformBlockSettingShader.glsl");

		m_CommitShadowShader = new Shader("assets/shaders/Shadow/vertCommitShadowShader.glsl", "assets/shaders/Shadow/fragCommitShadowShader.glsl");
		m_ViewShadowShader = new Shader("assets/shaders/Shadow/vertViewShadowShader.glsl", "assets/shaders/Shadow/fragViewShadowShader.glsl");
		m_DrawShadowShader = new Shader("assets/shaders/Shadow/vertShadowSceneShader.glsl", "assets/shaders/Shadow/fragShadowSceneShader.glsl");
		
		m_PBRShader = new Shader("assets/shaders/PBR/vertCookTorranceShader.glsl", "assets/shaders/PBR/fragCookTorranceShader.glsl");
		
		m_CommitTransparentShader = new Shader("assets/shaders/OIT/vertBuildListShader.glsl", "assets/shaders/OIT/fragBuildListShader.glsl");
		m_ResolveTransparentShader = new Shader("assets/shaders/OIT/vertResolveListShader.glsl", "assets/shaders/OIT/fragResolveListShader.glsl");

		// Retrieve uniform block location
		GLint camPrtiesLocation = glGetUniformBlockIndex(*m_UBOSettingShader, "CameraProperties");
		glUniformBlockBinding(*m_CommitShadowShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_ViewShadowShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_DrawShadowShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_PBRShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_CommitTransparentShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_ResolveTransparentShader, camPrtiesLocation, 24);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOCameraPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOCameraPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraProperties), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 24, m_UBOCameraPrties);

		// Retrieve uniform block location
		GLint lightPrtiesLocation = glGetUniformBlockIndex(*m_UBOSettingShader, "LightProperties");
		glUniformBlockBinding(*m_CommitShadowShader, lightPrtiesLocation, 25);
		glUniformBlockBinding(*m_ViewShadowShader, lightPrtiesLocation, 25);
		glUniformBlockBinding(*m_DrawShadowShader, lightPrtiesLocation, 25);
		glUniformBlockBinding(*m_PBRShader, lightPrtiesLocation, 25);
		glUniformBlockBinding(*m_CommitTransparentShader, lightPrtiesLocation, 25);
		glUniformBlockBinding(*m_ResolveTransparentShader, lightPrtiesLocation, 25);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOLightPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLightPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightProperties), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 25, m_UBOLightPrties);

		CreateOITBuffers();
		CreateDepthBuffer();

		return true;
	}

	void ThesisApp::Update(double gt)
	{
		RefresOITBuffers();

		// Animate here camera and light...

		// Update light and camera position.
		m_Camera->UpdateUniformBlock(m_UBOCameraPrties);	// Fill camera uniform block buffer
		m_LightSrc->UpdateUniformBlock(m_UBOLightPrties);	// Fill light uniform block buffer
	}

	void ThesisApp::Draw(double gt)
	{
		glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);

		DrawShadow();
		DrawOpaque();
		// DrawTransparents();
	}

	void ThesisApp::DrawShadow()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);

		// use proper program
		glUseProgram(*m_CommitShadowShader);

		// Bind the depth only FBO and set the viewport to the size of the depth texture
		glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFBO);
		glViewport(0, 0, 2048, 2048);

		// Clear
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Enable polygon offset to resolve depth-fighting issue
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(2.0f, 4.0f);

		// Draw geometries...
		m_CommitShadowShader->SetUniformMatrix4("uModel", 
			translate(mat4(1.0f), vec3(0.0f))
		);
		m_SmallOpaqueSphere->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel",
			translate(mat4(1.0f), vec3(1.0f))
		);
		m_MediumOpaqueSphere->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", 
			translate(mat4(1.0f), vec3(0.0f, -1.5f, 0.0f)) * 
			rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(3.0f, 3.0f, 1.0f))
		);
		m_GroundPlane->Draw();

		glDisable(GL_POLYGON_OFFSET_FILL);

		// Restore the default frame buffer and field of view
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_ClientWidth, m_ClientHeight);

		glUseProgram(*m_ViewShadowShader);
		glActiveTexture(GL_TEXTURE0);  
		glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
		m_ViewportPlane->Draw();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void ThesisApp::DrawOpaque()
	{
		// Now render from the viewer's position
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_TRUE);

		const mat4 scaleBiasMatrix = mat4(
			vec4(0.5f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, 0.5f, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.5f, 0.0f),
			vec4(0.5f, 0.5f, 0.5f, 1.0f)
		);

		// use proper program
		glUseProgram(*m_DrawShadowShader);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
		m_PBRShader->SetUniformMatrix4("uShadowMat", scaleBiasMatrix);
		 
		// Draw geometries...
		m_PBRShader->SetUniformMatrix4("uModel",
			translate(mat4(1.0f), vec3(0.0f))
		);
		m_SmallOpaqueSphere->Draw();

		m_PBRShader->SetUniformMatrix4("uModel",
			translate(mat4(1.0f), vec3(1.0f))
		);
		m_MediumOpaqueSphere->Draw();

		m_PBRShader->SetUniformMatrix4("uModel",
			translate(mat4(1.0f), vec3(0.0f, -1.5f, 0.0f)) *
			rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(3.0f, 3.0f, 1.0f))
		);
		m_GroundPlane->Draw();
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