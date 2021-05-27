#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "GL/glew.h"

namespace OpenGL
{
	/**
	 * Camera Properties.
	 * CameraProperties struct allow us to have a reference to memory order in the 
	 * uniform object buffer.
	 * Update this struct if you need more camera's data.
	 * 
	 * @see		Camera
	 * @see		Camera::UpdateUniformBlock
	 * @warning If you update this struct make sure to update also the 
	 *			Camera::UpdateUniformBlock function. You have to update also the shader program
	 *			uniform buffer object, the UBO layout must match the order declared in 
	 *			this struct.
	 */
	struct CameraProperties
	{
		/** Camera model matrix. */
		glm::mat4 Model = glm::mat4(1.0f);

		/** Camera view matrix. */
		glm::mat4 View = glm::mat4(1.0f);

		/** Camera projection matrix. */
		glm::mat4 Projection = glm::mat4(1.0f);

		/** Camera world position. */
		glm::vec4 Position = glm::vec4(0.0f);

		/** Camera forward. */
		glm::vec4 Direction = glm::vec4(0.0f);
	};

	/**
	 * Camera works as the human eye. All geometries inside its view bounds will be rendered. It offers 3 4x4 matrices
	 * useful to shading calculation, they are: model, view, projection. Those three matrices are also called MVP.
	 * In OpenGL the multiplication of this matrices is computed backwards, so we end up with PVM. Usually the camera
	 * matrices needed in shading are view and projection. The model one is used only to calculate cube maps or HDR.
	 * 
	 * @author	Alessandro Pantano
	 */
	class Camera
	{
	public:
		/**
		 * Camera constructor.
		 * 
		 * @param initialPosition	Pass the start camera's world position
		 * @param fovy				Pass the camera's vertical field of view
		 * @param aspectRatio		Pass the camera's aspect ratio
		 * @param near				Pass the camera's near plane
		 * @param far				Pass the camera's far plane
		 */
		Camera(glm::vec4 initialPosition, float fovy, float aspectRatio, float near, float far);

		/**
		 * Update the uniform block properties.
		 * 
		 * @param UBO ID of the uniform buffer generated
		 */
		void UpdateUniformBlock(GLuint ubo);

		/**
		 * Return the camera model matrix.
		 * 
		 * @return the copy-value to the translation matrix of camera position
		 */
		const glm::mat4 GetModelMat()	const { return glm::translate(glm::mat4(1.0f), glm::vec3(WorldPos)); }

		/**
		 * Return the camera view matrix.
		 *
		 * @return the reference to the camera view matrix
		 */
		const glm::mat4& GetViewMat()	const { return ViewMat; }

		/**
		 * Return the camera projection matrix.
		 * 
		 * @return the reference to the camera projection matrix
		 */
		const glm::mat4& GetProjMat()	const { return ProjMat; }

		/**
		 * Return the camera world position.
		 * 
		 * @return the reference to the camera position vector3
		 */
		const glm::vec3& GetWorldPos() const { return WorldPos; }

		/**
		 * Return the camera forward.
		 * 
		 * @return the reference to the camera position vector
		 */
		const glm::vec4 GetForward() const { return glm::transpose(ViewMat)[2]; }

		/**
		 * Set the light world position.
		 * 
		 * @param worldPos	world position vec3
		 */
		void SetWorldPos(glm::vec4 worldPos);

	public:
		/** Camera view matrix. By default lookat() at origin (0,0,0) */
		glm::mat4 ViewMat		= glm::mat4(1.0f);

		/** Camera projection matrix. */
		glm::mat4 ProjMat		= glm::mat4(1.0f);

		/** Camera world position vec3. */
		glm::vec4 WorldPos	= glm::vec4(0.0f, 0.0f, 4.0f, 0.0f);
	};
}