#include "ThesisApp.h"
#include "../assets/meshes/Plane.h"
#include "../assets/meshes/Sphere.h"
#include <iostream>

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

		delete m_ViewportPlane;

		delete m_GroundPlane;

		delete m_SmallOpaqueSphere;
		delete m_MediumOpaqueSphere;
		delete m_LargeOpaqueSphere;
		delete m_SmallOpaqueSphereTwo;
		delete m_MediumOpaqueSphereTwo;

		delete m_SmallTransparentSphere;
		delete m_MediumTransparentSphere;
		delete m_LargeTransparentSphere;
		delete m_SmallTransparentSphereTwo;
		delete m_MediumTransparentSphereTwo;

		delete m_CommitShadowShader;
		delete m_ViewShadowShader;

		delete m_PBRShader;

		delete m_CommitTransparentShader;
		delete m_ResolveTransparentShader;

		delete m_PG_Albedo;
		delete m_PG_AO;
		delete m_PG_Height;
		delete m_PG_Metallic;
		delete m_PG_Normal;
		delete m_PG_Roughness;
		delete m_CD_Albedo;
		delete m_CD_AO;
		delete m_CD_Height;
		delete m_CD_Metallic;
		delete m_CD_Normal;
		delete m_CD_Roughness;
		delete m_SBW_Albedo;
		delete m_SBW_AO;
		delete m_SBW_Height;
		delete m_SBW_Metallic;
		delete m_SBW_Normal;
		delete m_SBW_Roughness;
		delete m_GG_Albedo;
		delete m_GG_AO;
		delete m_GG_Metallic;
		delete m_GG_Normal;
		delete m_GG_Roughness;
	}

	bool ThesisApp::Initialize()
	{
		OpenGLApp::Initialize();

		// Initialize resources...

		m_LightSrc = new Light(vec4(0.0f, 10.0f, 0.2f, 0.0f), vec4(0.0f), vec4(5.0f, 4.5f, 4.3f, 0.8f), vec4(1.0f));
		m_Camera = new Camera(vec4(4.0f, 10.0f, 5.0f, 0.0f), 1.0472f, GetAspectRatio(), 0.1f, 100.0f);

		m_GroundPlane = new Plane();

		m_SmallOpaqueSphere		= new Sphere(256);
		m_SmallOpaqueSphereTwo	= new Sphere(256);
		m_MediumOpaqueSphere	= new Sphere(256);
		m_MediumOpaqueSphereTwo = new Sphere(256);
		m_LargeOpaqueSphere		= new Sphere(256);
		m_SmallTransparentSphere		= new Sphere(64);
		m_SmallTransparentSphereTwo		= new Sphere(64);
		m_MediumTransparentSphere		= new Sphere(64);
		m_MediumTransparentSphereTwo	= new Sphere(64);
		m_LargeTransparentSphere		= new Sphere(64);
		m_ViewportPlane = new Plane();

		m_UBOSettingShader			= new Shader("assets/shaders/vertUniformBlockSettingsShader.glsl", "assets/shaders/fragUniformBlockSettingShader.glsl");
		m_CommitShadowShader		= new Shader("assets/shaders/Shadow/vertCommitShadowShader.glsl",	"assets/shaders/Shadow/fragCommitShadowShader.glsl");
		m_ViewShadowShader			= new Shader("assets/shaders/Shadow/vertViewShadowShader.glsl",		"assets/shaders/Shadow/fragViewShadowShader.glsl");
		m_PBRShader					= new Shader("assets/shaders/PBR/vertCookTorranceShader.glsl",		"assets/shaders/PBR/fragCookTorranceShader.glsl");
		m_CommitTransparentShader	= new Shader("assets/shaders/OIT/vertBuildListShader.glsl",			"assets/shaders/OIT/fragBuildListShader.glsl");
		m_ResolveTransparentShader	= new Shader("assets/shaders/OIT/vertResolveListShader.glsl",		"assets/shaders/OIT/fragResolveListShader.glsl");

		m_PG_Albedo		= new Texture2D("assets/textures/pirate-gold/pirate-gold_albedo.png");
		m_PG_AO			= new Texture2D("assets/textures/pirate-gold/pirate-gold_ao.png");
		m_PG_Height		= new Texture2D("assets/textures/pirate-gold/pirate-gold_height.png");
		m_PG_Metallic	= new Texture2D("assets/textures/pirate-gold/pirate-gold_metallic.png");
		m_PG_Normal		= new Texture2D("assets/textures/pirate-gold/pirate-gold_normal-dx.png");
		m_PG_Roughness	= new Texture2D("assets/textures/pirate-gold/pirate-gold_roughness.png");

		m_CD_Albedo		= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_albedo.png");
		m_CD_AO			= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_ao.png");
		m_CD_Height		= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_height.png");
		m_CD_Metallic	= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_metallic.png");
		m_CD_Normal		= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_normal-dx.png");
		m_CD_Roughness	= new Texture2D("assets/textures/cavern-deposits/cavern-deposits_roughness.png");

		m_SBW_Albedo	= new Texture2D("assets/textures/stone-block-wall/sbw_albedo.png");
		m_SBW_AO		= new Texture2D("assets/textures/stone-block-wall/sbw_ao.png");
		m_SBW_Height	= new Texture2D("assets/textures/stone-block-wall/sbw_height.png");
		m_SBW_Metallic	= new Texture2D("assets/textures/stone-block-wall/sbw_metallic.png");
		m_SBW_Normal	= new Texture2D("assets/textures/stone-block-wall/sbw_normal-dx.png");
		m_SBW_Roughness		= new Texture2D("assets/textures/stone-block-wall/sbw_roughness.png");

		m_GG_Albedo		= new Texture2D("assets/textures/grey-granite/ggf-albedo.png");
		m_GG_AO			= new Texture2D("assets/textures/grey-granite/ggf-ao.png");
		m_GG_Metallic	= new Texture2D("assets/textures/grey-granite/ggf-Metallic.png");
		m_GG_Normal		= new Texture2D("assets/textures/grey-granite/ggf-Normal-dx.png");
		m_GG_Roughness	= new Texture2D("assets/textures/grey-granite/ggf-Roughness.png");

		// Retrieve uniform block location
		GLint camPrtiesLocation = glGetUniformBlockIndex(*m_UBOSettingShader, "CameraProperties");
		glUniformBlockBinding(*m_CommitShadowShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_ViewShadowShader, camPrtiesLocation, 24);
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
		m_Camera->SetWorldPos(vec4(sinf((float)gt / 7.0f) * 5.0f, sinf((float)gt / 2.0f) * 2.0f + 6.0f, cosf((float)gt / 10.0f) * 5.0f, 0.0f));
		m_LightSrc->SetWorldPos(vec4(cosf((float)gt / 7.0f) * 5.0f, sinf((float)gt / 2.0f) * 2.0f + 10.0f, sinf((float)gt / 10.0f) * 5.0f, 0.0f));

		// Update light and camera position.
		m_Camera->UpdateUniformBlock(m_UBOCameraPrties);	// Fill camera uniform block buffer
		m_LightSrc->UpdateUniformBlock(m_UBOLightPrties);	// Fill light uniform block buffer
	}

	void ThesisApp::Draw(double gt)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		DrawShadow(gt);	// Comments the two lines below to see shadow map result.
		DrawOpaque(gt);
		DrawTransparents(gt);
	}

	void ThesisApp::DrawShadow(double gt)
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
		m_CommitShadowShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(0.5f * pi<float>()) * 3.0f, 0.0f, cos(0.5f * pi<float>()) * 3.0f)));
		m_SmallOpaqueSphere->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(1.0f * pi<float>()) * 3.0f, 0.0f, cos(1.0f * pi<float>()) * 3.0f)));
		m_MediumOpaqueSphere->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(1.5f * pi<float>()) * 3.0f, 0.0f, cos(1.5f * pi<float>()) * 3.0f)));
		m_LargeOpaqueSphere->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(2.0f * pi<float>()) * 3.0f, 0.0f, cos(2.0f * pi<float>()) * 3.0f)));
		m_SmallOpaqueSphereTwo->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)));
		m_MediumOpaqueSphereTwo->Draw();

		m_CommitShadowShader->SetUniformMatrix4("uModel", 
			translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f)) * 
			rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(3.0f, 3.0f, 1.0f))
		);
		m_GroundPlane->Draw();

		glDisable(GL_POLYGON_OFFSET_FILL);

		// Restore the default frame buffer and field of view
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_ClientWidth, m_ClientHeight);

		glUseProgram(*m_ViewShadowShader);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
		m_ViewportPlane->Draw();
	}

	void ThesisApp::DrawOpaque(double gt)
	{
		// Now render from the viewer's position
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const mat4 scaleBiasMatrix = mat4(
			vec4(0.5f, 0.0f, 0.0f, 0.0f),
			vec4(0.0f, 0.5f, 0.0f, 0.0f),
			vec4(0.0f, 0.0f, 0.5f, 0.0f),
			vec4(0.5f, 0.5f, 0.5f, 1.0f)
		);

		// use proper program
		glUseProgram(*m_PBRShader);
		glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);

		m_PBRShader->SetUniformMatrix4("uShadowMat", scaleBiasMatrix);
		 
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_GG_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_GG_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_GG_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_GG_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_GG_Roughness);
		m_PBRShader->SetUniformFloat("uTilingFactor", 1.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.0f);
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3((float)sin(0.5f * pi<float>()) * 3.0f, 0.0f, (float)cos(0.5f * pi<float>()) * 3.0f)));
		m_SmallOpaqueSphere->Draw();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_SBW_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_SBW_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_SBW_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_SBW_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_SBW_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_SBW_Roughness);
		m_PBRShader->SetUniformFloat("uTilingFactor", 3.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", (float)(sin(gt / pi<float>()) * 0.5f + 0.5f) * 0.233f);
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3((float)sin(1.0f * pi<float>()) * 3.0f, 0.0f, (float)cos(1.0f * pi<float>()) * 3.0f)));
		m_MediumOpaqueSphere->Draw();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_GG_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_GG_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_GG_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_GG_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_GG_Roughness);
		m_PBRShader->SetUniformFloat("uTilingFactor", 3.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.0f);
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3((float)sin(1.5f * pi<float>()) * 3.0f, 0.0f, (float)cos(1.5f * pi<float>()) * 3.0f)));
		m_LargeOpaqueSphere->Draw();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_SBW_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_SBW_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_SBW_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_SBW_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_SBW_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_SBW_Roughness);
		m_PBRShader->SetUniformFloat("uTilingFactor", 5.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", (float)(sin(gt / pi<float>()) * 0.5f + 0.5f) * 0.3f);
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3((float)sin(2.0f * pi<float>()) * 3.0f, 0.0f, (float)cos(2.0f * pi<float>()) * 3.0f)));
		m_SmallOpaqueSphereTwo->Draw();

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_PG_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_PG_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_PG_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_PG_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_PG_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_PG_Roughness);
		m_PBRShader->SetUniformFloat("uTilingFactor", 3.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.02888f);
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)));
		m_MediumOpaqueSphereTwo->Draw();

		m_PBRShader->SetUniformFloat("uTilingFactor", 1.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.23312f);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_CD_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_CD_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_CD_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_CD_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_CD_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_CD_Roughness);
		m_PBRShader->SetUniformMatrix4("uModel",
			translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f)) *
			rotate(mat4(1.0f), radians(90.0f), vec3(-1.0f, 0.0f, 0.0f)) *
			scale(mat4(1.0f), vec3(5.0f, 5.0f, 1.0f))
		);
		m_GroundPlane->Draw();
	}

	void ThesisApp::DrawTransparents(double gt)
	{
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// use proper program
		glUseProgram(*m_CommitTransparentShader);

		// Draw geometries...
		m_CommitTransparentShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(0.25f * pi<float>()) * 3.0f, 0.0f, cos(0.25f * pi<float>()) * 3.0f)));
		m_CommitTransparentShader->SetUniformVec4("uColor", vec4(sin(0.25f * pi<float>()) * 3.0f, gt, (float)cos(0.25f * pi<float>()) * 3.0f, (sin(gt / pi<float>()) * 0.5f + 0.5f) * 0.9f));
		m_SmallTransparentSphere->Draw();

		m_CommitTransparentShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(0.75f * pi<float>()) * 3.0f, 0.0f, cos(0.75f * pi<float>()) * 3.0f)));
		m_CommitTransparentShader->SetUniformVec4("uColor", vec4(sin(0.75f * pi<float>()) * 3.0f, gt, (float)cos(0.75f * pi<float>()) * 3.0f, (cos(gt / pi<float>()) * 0.5f + 0.5f) * 0.7f));
		m_MediumTransparentSphere->Draw();

		m_CommitTransparentShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(1.25f * pi<float>()) * 3.0f, 0.0f, cos(1.25f * pi<float>()) * 3.0f)));
		m_CommitTransparentShader->SetUniformVec4("uColor", vec4(sin(1.25f * pi<float>()) * 3.0f, gt, (float)cos(1.25f * pi<float>()) * 3.0f, (sin(gt / pi<float>()) * 0.5f + 0.5f) * 0.5f));
		m_LargeTransparentSphere->Draw();

		m_CommitTransparentShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(sin(1.75f * pi<float>()) * 3.0f, 0.0f, cos(1.75f * pi<float>()) * 3.0f)));
		m_CommitTransparentShader->SetUniformVec4("uColor", vec4(sin(1.75f * pi<float>()) * 3.0f, gt, (float)cos(1.75f * pi<float>()) * 3.0f, (cos(gt / pi<float>()) * 0.5f + 0.5f) * 0.98f));
		m_SmallTransparentSphereTwo->Draw();

		m_CommitTransparentShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.5f, 1.5f, 1.5f)));
		m_CommitTransparentShader->SetUniformVec4("uColor", vec4(0.5f, 0.6f, 0.8f, (float)sin(gt / 4.0f) * 0.5f + 0.5f));
		m_MediumTransparentSphereTwo->Draw();

		// use proper program
		glUseProgram(*m_ResolveTransparentShader);

		// Render it to a plane.
		m_ViewportPlane->Draw();
	}
}