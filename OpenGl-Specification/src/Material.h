#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace OpenGL
{
	/**
	 * Object materials can be simulated in an ADS lighting model by specifying
	 * four values:
	 * 
	 * - ambient, simulates a low-level illumination that equally affects everything in the scene. 
	 * - diffuse, brightens objects to various degrees depending on the light's angle of incidence.
	 * - specular, coveys the shininess of an object by strategically placing a highlight of appropriate 
	 *				size on the object's surface where light is reflected most directly towards our eyes.
	 * - shininess, which is used to build an appropriate specular highlight for the intended material.
	 * 
	 * ADS and shininess values have been developed for many different types of common materials.
	 */
	class Material
	{
	public:
		Material() = default;

	public:
		virtual void CommitToProgram(Shader& shader);
			
	protected:
		glm::vec3 m_GlobalAmbient = glm::vec3(.7f, .7f, .7f);
		glm::vec3 m_Ambient = glm::vec3(.11f, .06f, .11f);
		glm::vec3 m_Diffuse = glm::vec3(.43f, .47f, .54f);
		glm::vec3 m_Specular = glm::vec3(.33f, .33f, .52f);
		float m_Shininess = 9.85f;
		// float m_Trasparency = 1.0f; Used for some materials. For now I leave it as unused.
	};

	class PewterMaterial : public Material
	{
	public:
		PewterMaterial();
	};

	class GoldMaterial : public Material
	{
	public:
		GoldMaterial();
	};

	class SilverMaterial : public Material
	{
	public:
		SilverMaterial();
	};

	class BronzeMaterial : public Material
	{
	public:
		BronzeMaterial();
	};
}