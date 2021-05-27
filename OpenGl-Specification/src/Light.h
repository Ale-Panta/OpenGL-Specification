#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace OpenGL
{
	/**
	 * Light Properties. 
	 * LightProperties struct allow us to have a reference to memory order in the uniform object buffer.
	 * Update this struct if you need more light's data.
	 * 
	 * @see		Light::UpdateUniformBlock
	 * @warning If you update this struct make sure to update also the
	 *			Light::UpdateUniformBlock function. You have to update also the shader program
	 *			uniform buffer object, the UBO layout must match the order declared in
	 *			this struct.
	 * @author	Alessandro Pantano
	 */
	struct LightProperties
	{
		/** Light model matrix */
		glm::mat4 Model = glm::mat4(1.0f);

		/** Light model matrix */
		glm::mat4 View = glm::mat4(1.0f);

		/** Light model matrix */
		glm::mat4 Projection = glm::mat4(1.0f);

		/** Light source world position */
		glm::vec4 Position	= glm::vec4(0.0f);

		/** Light source world position */
		glm::vec4 Direction = glm::vec4(0.0f);

		/** Light source color emitted */
		glm::vec4 Color		= glm::vec4(0.0f);

		/** Ambient color */
		glm::vec4 Ambient	= glm::vec4(0.0f);	// It's not part of the light property but I choose to put it here anyway.
	};

	/**
	 * Light class is similar to camera class. It offer MVP matrices that can be used for shadow mapping.
	 * It usually includes other properties such as ambient, diffuse and specular colors. 
	 * 
	 * @see		Camera
	 * @author	Alessandro Pantano
	 */
	class Light
	{
	public:
		/**
		 * Light constructor.
		 * 
		 * @param worldPos		light's start world position.
		 * @param ambientCol	light's start ambient color.
		 * @param diffuseCol	light's start diffuse color.
		 * @param specularCol	light's start specular color.
		 */
		Light(glm::vec4 worldPos, glm::vec4 ambientCol, glm::vec4 diffuseCol, glm::vec4 specularCol);

		/**
		 * Update the uniform block properties.
		 * 
		 * @param ubo ID of the uniform buffer generated
		 */
		void UpdateUniformBlock(GLuint ubo);

		/**
		 * Return the light model matrix 4x4.
		 * 
		 * @return A copy to the translation matrix of light position
		 */
		const glm::mat4 GetModelMat()	const { return glm::translate(glm::mat4(1.0f), glm::vec3(WorldPos)); }

		/**
		 * Return the light view matrix 4x4.
		 * 
		 * @return The reference to the light view matrix
		 */
		const glm::mat4& GetViewMat()	const { return ViewMat; }

		/**
		 * Return the light projection matrix 4x4.
		 * 
		 * @return The reference to the light projection matrix
		 */
		const glm::mat4& GetProjMat()	const { return ProjMat; }

		/**
		 * Return the light forward.
		 * 
		 * @return A copy to the second raw of the view matrix transpose
		 */
		const glm::vec4 GetForward() const { return glm::transpose(ViewMat)[2]; }

		/**
		 * Set the light world position.
		 * 
		 * @param worldPos world position vec3
		 */
		void SetWorldPos(glm::vec4 worldPos);

	public:
		/** Light world position. By default is positioned in vec3(4.0f), top right corner. */
		glm::vec4 WorldPos		= glm::vec4(4.0f);

		/** Light ambient color. This influence the the model's color of the zone that is not facing the light source. */
		glm::vec4 AmbientCol	= glm::vec4(0.0f);

		/** Light diffuse color. It is the color that the model revels under pure white light. */
		glm::vec4 DiffuseCol	= glm::vec4(1.0f);

		/** Light specular color. Color of the light of a specular reflection. */
		glm::vec4 SpecularCol	= glm::vec4(1.0f);

		/** Light view matrix. */
		glm::mat4 ViewMat		= glm::mat4(1.0f);

		/** Light projection matrix. */
		glm::mat4 ProjMat		= glm::mat4(1.0f);
	};
}