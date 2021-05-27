#pragma once

#include <GL/glew.h>

namespace OpenGL
{
	/**
	 * Texture type internal format.
	 */
	enum class TextureType
	{
		Diffuse = GL_TEXTURE_2D, 
		CubeMap = GL_TEXTURE_CUBE_MAP, 
	};

	/**
	 * Parent class of all type of texture. Here are stored the most common properties shared among most
	 * of the texture assets.
	 * 
	 * @author	Alessandro Pantano
	 */
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		/** 
		 * Return the reference to texture type.
		 * 
		 * @see #TextureType
		 */
		const TextureType& GetTextureType() const { return m_TexType; }

		operator GLuint() { return m_ID; }

	protected:
		GLuint m_ID		= 0;
		int m_ClientWidth		= 0;
		int m_ClientHeight	= 0;
		TextureType m_TexType = TextureType::Diffuse;
	};

	/**
	 * 2D texture asset. Use it to create any texture. Texture can be diffuse, specular and so on.
	 * 
	 * @author	Alessandro Pantano
	 */
	class Texture2D : public Texture
	{
	public:
		Texture2D() = default;

		/**
		 * Texture2D constructor.
		 * 
		 * @param filePath	asset path including extension
		 */
		Texture2D(const char* filePath);
	};

	/**
	 * Cube map read six different texture path and wrap all the six texture around the cube.
	 * 
	 * @author	Alessandro Pantano
	 */
	class TextureCubeMap : public Texture
	{
	public:
		TextureCubeMap() = default;

		/**
		 * TextureCubeMap constructor.
		 * 
		 * @param right		right texture file path
		 * @param left		left texture file path
		 * @param top		top texture file path
		 * @param bottom	bottom texture file path
		 * @param front		front texture file path
		 * @param back		back texture file path
		 */
		TextureCubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
	};
}