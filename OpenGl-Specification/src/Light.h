#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace OpenGL
{
	/**
	 * LightProperties struct allow us to have a reference to memory order in the uniform object buffer.
	 * This struct is strictly related to shader program. Make sure to update both this struct and the shader.
	 * 
	 * @see Uniform Object Buffer (UBO).
	 */
	struct LightProperties
	{
		/** Light source world position */
		glm::vec4 Position	= glm::vec4(0.0f);

		/** Light source color emitted */
		glm::vec4 Color		= glm::vec4(0.0f);

		/** Ambient color */
		glm::vec4 Ambient	= glm::vec4(0.0f);	// It's not part of the light property but I choose to put it here anyway.
	};

	/**
	 * There are many type of lights, each with different characteristics and requiring different
	 * steps to simulate their effects. Some types include:
	 * @see Global (usually called "global ambient" because it includes only an ambient component).
	 * @see Directional (or "distant").
	 * @see Positional (or "point source" or "point light").
	 * @see Spotlight.
	 */

	/**
	 * Light class is similar to camera class. It offer MVP matrices that can be used for shadow mapping.
	 * It usually includes other properties such as ambient, diffuse and specular colors. 
	 * 
	 * @see OpenGL::Camera class
	 */
	class Light
	{
	public:
		/**
		 * Light constructor.
		 * 
		 * @param worldPos, light's start world position.
		 * @param ambientCol, light's start ambient color.
		 * @param diffuseCol, light's start diffuse color.
		 * @param specularCol, light's start specular color.
		 */
		Light(glm::vec4 worldPos, glm::vec4 ambientCol, glm::vec4 diffuseCol, glm::vec4 specularCol);

		/**
		 * Update the uniform block properties.
		 * 
		 * @param ubo, ID of the uniform buffer generated.
		 * @see uniform buffer object.
		 */
		void UpdateUniformBlock(GLuint ubo);

		/**
		 * Return the light model matrix 4x4.
		 *
		 * @return the reference to the translation matrix of light position.
		 */
		const glm::mat4& GetModelMat()	const { return glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos)); }

		/**
		 * Return the light view matrix 4x4.
		 *
		 * @return the reference to the light view matrix.
		 */
		const glm::mat4& GetViewMat()	const { return m_ViewMat; }


		/**
		 * Return the light projection matrix 4x4.
		 *
		 * @return the reference to the light projection matrix.
		 */
		const glm::mat4& GetProjMat()	const { return m_ProjMat; }

		/**
		 * Set the light world position
		 * 
		 * @param worldPos, world position vec3
		 */
		void SetWorldPos(glm::vec4 worldPos);

	private:
		/** Light world position. By default is positioned in vec3(4.0f), top right corner */
		glm::vec4 m_WorldPos	= glm::vec4(4.0f);

		/** Light ambient color. This influence the the model's color of the zone that is not facing the light source */
		glm::vec4 m_AmbientCol	= glm::vec4(0.0f);

		/** Light diffuse color. It is the color that the model revels under pure white light */
		glm::vec4 m_DiffuseCol	= glm::vec4(1.0f);

		/** Light specular color. Color of the light of a specular reflection. */
		glm::vec4 m_SpecularCol = glm::vec4(1.0f);

		/** Light view matrix */
		glm::mat4 m_ViewMat		= glm::mat4(1.0f);

		/** Light projection matrix */
		glm::mat4 m_ProjMat		= glm::mat4(1.0f);
	};

	/**
	 * Global ambient light is the simplest type of light to model. Global ambient light has no source
	 * position-the light is equal everywhere, at each pixel on every object in the scene, regardless
	 * of where the objects are. Global ambient lighting simulates the real-world phenomenon of light 
	 * that has bounced around so many times that its source and direction are indeterminable. Global
	 * ambient light has only an ambient component, specified as an RGBA value; it has no diffuse or
	 * specular components.
	 * RGBA values range from 0 to 1, so global ambient light is usually modeled as dim white light,
	 * where each of the RGB values is set to the same fractional value between 0 to 1 and the alpha is 
	 * set to 1.
	 */

	/**
	 * Directional light also doesn't have a source location, but it does have a direction.
	 * It is useful for situations where the source of the light is so far away that the light rays are
	 * effectively parallel, such as light coming from the sun. In many such situations we may only be 
	 * interested in modeling the light and not the object that produces the light. The effect of direction
	 * light on an object depends on the light's angle of impact; objects are brighter on the side facing
	 * a directional light than on a tangential or opposite side. Modeling directional light requires
	 * specifying its direction (as a vector) and its ambient, diffuse, and specular characteristics (RGBA).
	 * It might seem redundant to include an ambient contribution for a light when we already have a global
	 * ambient light. The separation of the two is intentional and noticeable when the light is on or off.
	 * When on the total ambient contribution would be increased. 
	 */

	/**
	 * Position light has a specific location in the 3D scene. Light sources that are near the scene, such as
	 * lamps, candles, and so forth, are examples. Like directional lights, the effect of a positional light
	 * depends on angle of impact; however, its direction is not specified, as it is different for each vertex
	 * in our scene. Positional lights may also incorporate attenuation factors in order to model how their
	 * intensity diminishes with distance. As with the other types of lights we have seen, positional lights have
	 * ambient, diffuse, and specular properties specified as RGBA values.
	 * Attenuation factors can be modeled in a variety of ways. One way is to include tunable non-negative
	 * parameters for constant, linear, and quadratic attenuation (Kc, Kl and Kq respectively). These parameters
	 * are the combined, taking into account the distance (d) from the light source.
	 *									1
	 * attenuationFactor = ---------------------------
	 *						Kc + (Kl * d) + (Kq * de2)
	 * Multiplying this factor by the light intensity causes the intensity to be decreased the greater the distance
	 * is to the light source.
	 * #NOTE that Kc should always be in the range [0, 1] and approach 0 as the distance d increased.
	 */

	/**
	 * Spotlight have both a position ans a direction. The effect of the spotlight's "cone" can be simulated
	 * using a cutoff angle theta between (0, 90) specifying the half-with of the light beam, and a falloff 
	 * exponent to model the variation of intensity across the angle of the beam.
	 */
}