#include "Texture.h"

#include <SOIL2.h>

namespace OpenGL
{
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_ID);
	}

	Texture2D::Texture2D(const char* filePath)
	{
		unsigned char* data = SOIL_load_image(filePath, &m_Width, &m_Height, nullptr, SOIL_LOAD_RGBA);

		glGenTextures(1, &m_ID);

		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Free up the memory.
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(data);
	}
}