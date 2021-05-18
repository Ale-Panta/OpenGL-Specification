#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "GL/glew.h"

namespace OpenGL
{
	/**
	 * Camera Properties.
	 * LightProperties struct allow us to have a reference to memory order in the uniform object buffer.
	 * Update this struct if you need more camera's data.
	 * @warning Make sure to update the UpdateUniformBlock function.
	 * @warning Make sure to update the shader program. The uniform buffer layout must match this struct order.
	 * @see UpdateUniformBlock.
	 */
	struct CameraProperties
	{
		/** Camera model matrix. */
		glm::mat4 M = glm::mat4(1.0f);

		/** Camera view matrix. */
		glm::mat4 V = glm::mat4(1.0f);

		/** Camera projection matrix. */
		glm::mat4 P = glm::mat4(1.0f);

		/** Camera world position. */
		glm::vec4 Position = glm::vec4(0.0f);
	};

	/**
	 * Camera works as the human eye. All geometries inside its view bounds will be rendered. It offers 3 4x4 matrices
	 * useful to shading calculation, they are: model, view, projection. Those three matrices are also called MVP.
	 * In OpenGL the multiplication of this matrices is computed backwards, so we end up with PVM. Usually the camera
	 * matrices needed in shading are view and projection. The model one is used only to calculate cube maps or HDR.
	 */
	class Camera
	{
	public:
		/**
		 * Camera constructor.
		 * @param initialPosition - camera's start world position.
		 * @param fovy - camera's start vertical field of view.
		 * @param aspectRatio - camera's start aspect ratio.
		 * @param near - camera's start near plane.
		 * @param far - camera's start far plane.
		 */
		Camera(glm::vec4 initialPosition, float fovy, float aspectRatio, float near, float far);

		/**
		 * Update the uniform block properties.
		 * @param ubo - ID of the uniform buffer generated.
		 */
		void UpdateUniformBlock(GLuint ubo);

		/**
		 * Return the camera model matrix 4x4.
		 * @return the copy-value to the translation matrix of camera position. 
		 */
		const glm::mat4 GetModelMat()	const { return glm::translate(glm::mat4(1.0f), glm::vec3(m_WorldPos)); }

		/**
		 * Return the camera view matrix 4x4.
		 * @return the reference to the camera view matrix.
		 */
		const glm::mat4& GetViewMat()	const { return m_ViewMat; }

		/**
		 * Return the camera projection matrix 4x4.
		 * @return the reference to the camera projection matrix.
		 */
		const glm::mat4& GetProjMat()	const { return m_ProjMat; }

		/**
		 * Return the camera world position vector3.
		 * @return the reference to the camera position vector3.
		 */
		const glm::vec3& GetWorldPos() const { return m_WorldPos; }

	private:
		/** Camera view matrix. By default lookat() at origin (0,0,0) */
		glm::mat4 m_ViewMat		= glm::mat4(1.0f);

		/** Camera projection matrix. */
		glm::mat4 m_ProjMat		= glm::mat4(1.0f);

		/** Camera world position vec3. */
		glm::vec4 m_WorldPos	= glm::vec4(0.0f, 0.0f, 4.0f, 0.0f);
	};
}