#pragma once
#include <GL/glew.h>

namespace OpenGL
{
	class Texture
	{
	public:
		static GLuint LoadTexture(const char* textureImagePath);
	};
}