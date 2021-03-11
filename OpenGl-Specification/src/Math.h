#pragma once

#include <GLM/glm.hpp>

namespace OpenGl
{
	namespace Math
	{
		using namespace glm;

		mat4 BuildTranslate(float x, float y, float z)
		{
			mat4 translation = mat4
			(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				x, y, z, 1.0
			);

			return translation;
		}

		mat4 BuildRotateX(float rad)
		{
			mat4 rot = mat4
			(
				1.0, 0.0, 0.0, 0.0,
				0.0, cos(rad), -sin(rad), 0.0,
				0.0, sin(rad), cos(rad), 0.0,
				0.0, 0.0, 0.0, 1.0
			);

			return rot;
		}

		mat4 BuildRotateY(float rad)
		{
			mat4 rot = mat4
			(
				cos(rad), 0.0, sin(rad), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rad), 0.0, cos(rad), 0.0,
				0.0, 0.0, 0.0, 1.0
			);

			return rot;
		}

		mat4 BuildRotateZ(float rad)
		{
			mat4 rot = mat4
			(
				cos(rad), -sin(rad), 0.0, 0.0,
				sin(rad), cos(rad), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);

			return rot;
		}

		mat4 BuildScale(float x, float y, float z)
		{
			mat4 scale = mat4
			(
				x, 0.0, 0.0, 0.0,
				0.0, y, 0.0, 0.0,
				0.0, 0.0, z, 0.0,
				0.0, 0.0, 0.0, 1.0
			);

			return scale;
		}
	}
}