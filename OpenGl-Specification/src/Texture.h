#pragma once

#include <GL/glew.h>

namespace OpenGL
{
	enum class TextureType
	{
		Diffuse = GL_TEXTURE_2D, 
		CubeMap = GL_TEXTURE_CUBE_MAP, 
	};

	/**
	 * Parent class of all type of texture. Here are stored the most common properties shared among most
	 * of the texture assets.
	 */
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		/** 
		 * Return the reference to texture type 
		 *
		 * @see OpenGL::TextureType
		 */
		const TextureType& GetTextureType() const { return m_TexType; }

		operator GLuint() { return m_ID; }

	protected:
		GLuint m_ID		= 0;
		int m_Width		= 0;
		int m_Height	= 0;
		TextureType m_TexType = TextureType::Diffuse;
	};

	/**
	 * 2D texture asset. Use it to create any texture. Texture can be diffuse, specular and so on...
	 */
	class Texture2D : public Texture
	{
	public:
		Texture2D() = default;

		/**
		 * Texture2D constructor.
		 * 
		 * @param filePath, asset path including extension
		 */
		Texture2D(const char* filePath);
	};

	/**
	 * Cube map read six different texture path and wrap all the six texture around the cube.
	 */
	class TextureCubeMap : public Texture
	{
	public:
		TextureCubeMap() = default;
		TextureCubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
	};

	/**
	 * Texture used to draw shadow. Here all geometry is rendered from the point of view of the light source.
	 * Each pixel that do not pass the depth test is stored in this texture. Note the texture is black and white.
	 * This texture is attached to a custom frame buffer, we can draw on it with this process.
	 * Shadows can be rendered sampling this texture.
	 */
	class TextureShadow : public Texture
	{
	public:
		TextureShadow() = default;

		/**
		 * TextureShadow constructor.
		 *
		 * @param frameBufferObject, FBO
		 * @param width, texture width
		 * @param height, texture height
		 */
		TextureShadow(GLuint& frameBufferObject, int width, int height);
	};
}