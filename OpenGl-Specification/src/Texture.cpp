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
		m_TexType = TextureType::Diffuse;

		unsigned char* m_data = SOIL_load_image(filePath, &m_ClientWidth, &m_ClientHeight, nullptr, SOIL_LOAD_RGBA);

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ClientWidth, m_ClientHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
			glGenerateTextureMipmap(m_ID);
		}

		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(m_data);
	}

	TextureCubeMap::TextureCubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back)
	{
		m_TexType = TextureType::CubeMap;

		m_ID = SOIL_load_OGL_cubemap(right, left, top, bottom, front, back, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}