#include "LitScene.h"
#include "../meshes/Sphere.h"
#include "../meshes/Plane.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

using namespace glm;
using namespace std;

namespace OpenGL
{

	LitScene::LitScene(float fovy, float aspectRatio, float near, float far)
	{
		m_Camera = new Camera(vec4(0.0f, 0.0f, 4.0f, 0.0f), fovy, aspectRatio, near, far);
		m_LightSource = new Light(vec4(0.0f, 0.0f, 5.0f, 0.0f), vec4(0.0f), vec4(50.0f, 45.0f, 43.0f, 1.0f), vec4(1.0f));
	}

	void LitScene::BeginScene(GLFWwindow* context)
	{
		m_SphereCD = new Sphere(256);
		m_SpherePG = new Sphere(256);
		m_SphereSS = new Sphere(256);
		m_SphereVM = new Sphere(256);

		m_LitShader = new Shader("assets/shaders/PBRCookTorrance/vertCookTorranceShader.glsl", "assets/shaders/PBRCookTorrance/fragCookTorranceShader.glsl");

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

		m_SS_Albedo		= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-albedo.png");
		m_SS_AO			= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-ao.png");
		m_SS_Height		= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-height.png");
		m_SS_Metallic	= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-metallic.png");
		m_SS_Normal		= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-normal-dx.png");
		m_SS_Roughness	= new Texture2D("assets/textures/spaceship-panels/spaceship-panels1-roughness.png");

		m_VM_Albedo		= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_albedo.png");
		m_VM_AO			= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_ao.png");
		m_VM_Height		= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_height.png");
		m_VM_Metallic	= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_metallic.png");
		m_VM_Normal		= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_normal-dx.png");
		m_VM_Roughness	= new Texture2D("assets/textures/vented-metal-panel/vented-metal-panel1_roughness.png");

		// Retrieve uniform block location
		GLint camPrtiesLocation = glGetUniformBlockIndex(*m_LitShader, "CameraProperties");
		glUniformBlockBinding(*m_LitShader, camPrtiesLocation, 24);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOCamPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOCamPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraProperties), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 24, m_UBOCamPrties);

		// Retrieve uniform block location
		GLint lightPrtiesLocation = glGetUniformBlockIndex(*m_LitShader, "LightProperties");
		glUniformBlockBinding(*m_LitShader, lightPrtiesLocation, 25);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOLightPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLightPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightProperties), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 25, m_UBOLightPrties);

		m_Camera->UpdateUniformBlock(m_UBOCamPrties);			// Fill camera uniform block buffer
		m_LightSource->UpdateUniformBlock(m_UBOLightPrties);	// Fill light uniform block buffer
	}

	void LitScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		glClearColor(.09f, .088f, .092f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void LitScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void LitScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void LitScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		static float lightDistance = 5.0f;
		m_LightSource->SetWorldPos(vec4(sin(currentTime / 5.0f) * lightDistance, 0.0f, cos(currentTime / 5.0f) * lightDistance, 0.0f));
		m_LightSource->UpdateUniformBlock(m_UBOLightPrties);

		uint8 cycleCount = (uint8)((float)currentTime / 10.0f) % 4;

		switch (cycleCount)
		{
		case 0:
		{
			m_LitShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
			m_LitShader->SetUniformFloat("uTilingFactor", 3.0f);
			m_LitShader->SetUniformFloat("uDisplacementFactor", 0.0333f);

			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_PG_Albedo);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_PG_AO);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_PG_Height);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_PG_Metallic);
			glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_PG_Normal);
			glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_PG_Roughness);

			m_SpherePG->Draw(*m_LitShader);
		}
		break;
		case 1:
		{
			m_LitShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
			m_LitShader->SetUniformFloat("uTilingFactor", 1.0f);
			m_LitShader->SetUniformFloat("uDisplacementFactor", 0.333f);

			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_CD_Albedo);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_CD_AO);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_CD_Height);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_CD_Metallic);
			glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_CD_Normal);
			glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_CD_Roughness);

			m_SphereCD->Draw(*m_LitShader);
		}
		break;
		case 2:
		{
			m_LitShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
			m_LitShader->SetUniformFloat("uTilingFactor", 6.0f);
			m_LitShader->SetUniformFloat("uDisplacementFactor", 0.2222f);

			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_SS_Albedo);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_SS_AO);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_SS_Height);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_SS_Metallic);
			glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_SS_Normal);
			glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_SS_Roughness);

			m_SphereSS->Draw(*m_LitShader);
		}
		break;
		case 3:
		{
			m_LitShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
			m_LitShader->SetUniformFloat("uTilingFactor", 2.0f);
			m_LitShader->SetUniformFloat("uDisplacementFactor", 0.03f);

			glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_VM_Albedo);
			glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_VM_AO);
			glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_VM_Height);
			glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_VM_Metallic);
			glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_VM_Normal);
			glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_VM_Roughness);

			m_SphereVM->Draw(*m_LitShader);
		}
		break;
		}
	}
}