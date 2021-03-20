#pragma once

#include <GL/glew.h>

namespace OpenGL
{
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

	public:
		operator GLuint() { return m_ID; }

	protected:
		GLuint m_ID		= 0;
		int m_Width		= 0;
		int m_Height	= 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D() = default;
		Texture2D(const char* filePath);
	};
}