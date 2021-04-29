#include "PBRScene.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

using namespace glm;
using namespace std;

namespace OpenGL
{

	void PBRScene::BeginScene(GLFWwindow* context)
	{
		m_Camera = make_shared<Camera>(vec3(.0f, .0f, 3.0f), .01f);
		
		m_LightPosition = make_shared<vec3>(0.0f, 0.0f, 5.0f);
		m_LightColor = make_shared<vec3>(50.0f, 45.0f, 43.0f);

		m_SphereCD = make_shared<Sphere>(128);
		m_SpherePG = make_shared<Sphere>(128);
		m_SphereSS = make_shared<Sphere>(128);
		m_SphereVM = make_shared<Sphere>(128);

		m_PBRShader = make_shared<Shader>("assets/shaders/PBRCookTorrance/vertCookTorranceShader.glsl", "assets/shaders/PBRCookTorrance/fragCookTorranceShader.glsl");

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

		LitProperties properties = {m_Camera->GetModelMatrix(), m_Camera->GetViewMatrix(), m_Camera->GetProjMatrix(), vec4(m_Camera->GetPosition(), 1.0f), vec4(*m_LightPosition, 1.0f), vec4(*m_LightColor, 1.0f), vec4(0.0f)};

		m_LitPropertiesLocation = glGetUniformBlockIndex(*m_PBRShader, "LitProperties");
		glUniformBlockBinding(*m_PBRShader, m_LitPropertiesLocation, 13);

		glGenBuffers(1, &m_UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LitProperties), &properties, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, 13, m_UBO);
	}

	void PBRScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		glClearColor(.09f, .088f, .092f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void PBRScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		vec4 lightPosition = vec4(sin(currentTime * 0.5) * 5.0f, 0.0f, cos(currentTime * 0.5) * 5.0f, 0.0f);

		// Update sub data of uniform block every frame
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(LitProperties, LitProperties::LightPos), sizeof(vec4), value_ptr(lightPosition));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		/*
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
		m_PBRShader->SetUniformFloat("uTilingFactor", 3.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.0333f);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_PG_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_PG_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_PG_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_PG_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_PG_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_PG_Roughness);

		m_SpherePG->Draw(*m_PBRShader);
		*/

		/*
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
		m_PBRShader->SetUniformFloat("uTilingFactor", 1.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.333f);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_CD_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_CD_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_CD_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_CD_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_CD_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_CD_Roughness);

		m_SphereCD->Draw(*m_PBRShader);
		*/

		/*
		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
		m_PBRShader->SetUniformFloat("uTilingFactor", 4.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.2222f);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_SS_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_SS_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_SS_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_SS_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_SS_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_SS_Roughness);

		m_SphereSS->Draw(*m_PBRShader);

		m_PBRShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)));
		m_PBRShader->SetUniformFloat("uTilingFactor", 2.0f);
		m_PBRShader->SetUniformFloat("uDisplacementFactor", 0.03f);

		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, *m_VM_Albedo);
		glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, *m_VM_AO);
		glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, *m_VM_Height);
		glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, *m_VM_Metallic);
		glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, *m_VM_Normal);
		glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, *m_VM_Roughness);

		m_SphereVM->Draw(*m_PBRShader);
		*/
	}

}