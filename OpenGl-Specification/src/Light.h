#pragma once

#include "Shader.h"

#include <glm/glm.hpp>

namespace OpenGL
{
	/**
	 * There are many type of lights, each with different characteristics and requiring different
	 * steps to simulate their effects. Some types include:
	 * - Global (usually called "global ambient" because it includes only an ambient component).
	 * - Directional (or "distant").
	 * - Positional (or "point source" or "point light").
	 * - Spotlight
	 */
	class Light
	{
	public:
		Light() = default;

	public:
		void CommitToShader(Shader& shader);

	private:
		glm::vec3 m_Position = glm::vec3(10.0f, 0.0f, -4.0f);
		glm::vec3 m_Ambient	= glm::vec3(.0f, .0f, .0f);
		glm::vec3 m_Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	};

	/**
	 * Global ambient light is the simplest type of light to model. Global ambient light has no source
	 * position-the light is equal everywhere, at each pixel on every object in the scene, regardless
	 * of where the objects are. Global ambient lighting simulates the real-world phenomenon of light 
	 * that has bounced around so many times that its source and direction are undeterminable. Global
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