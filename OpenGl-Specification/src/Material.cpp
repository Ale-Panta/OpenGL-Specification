#include "MAterial.h"

using namespace glm;

namespace OpenGL
{
	void Material::CommitToProgram(Shader& shader)
	{
		shader.SetUniformVec3("uMaterial.globalAmbient", m_GlobalAmbient);
		shader.SetUniformVec3("uMaterial.ambient", m_Ambient);
		shader.SetUniformVec3("uMaterial.diffuse", m_Diffuse);
		shader.SetUniformVec3("uMaterial.specular", m_Specular);
		shader.SetUniformFloat("uMaterial.shininess", m_Shininess);
	}

	PewterMaterial::PewterMaterial()
	{
		m_GlobalAmbient = vec3(.7f, .7f, .7f);
		m_Ambient = vec3(.11f, .06f, .11f);
		m_Diffuse = vec3(.43f, .47f, .54f);
		m_Specular = vec3(.33f, .33f, .52f);
		m_Shininess = 9.85f;
	}

	GoldMaterial::GoldMaterial()
	{
		m_GlobalAmbient = vec3(.7f, .7f, .7f);
		m_Ambient = vec3(.24725f, .1995f, .0745f);
		m_Diffuse = vec3(.75164f, .60648f, .22648f);
		m_Specular = vec3(.62828f, .5558f, .36607f);
		m_Shininess = 51.2f;
	}

	SilverMaterial::SilverMaterial()
	{
		m_GlobalAmbient = vec3(.7f, .7f, .7f);
		m_Ambient = vec3(.19225f, .19225f, .19225f);
		m_Diffuse = vec3(.50754f, .50754f, .50754f);
		m_Specular = vec3(.50827f, .50827f, .50827f);
		m_Shininess = 51.2f;
	}

	BronzeMaterial::BronzeMaterial()
	{
		m_GlobalAmbient = vec3(.7f, .7f, .7f);
		m_Ambient = vec3(.2125f, .2125f, .2125f);
		m_Diffuse = vec3(.7140f, .4284f, .1814f);
		m_Specular = vec3(.3936f, .2719f, .1667f);
		m_Shininess = 25.6f;
	}
}