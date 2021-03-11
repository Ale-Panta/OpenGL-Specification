#include "Texture.h"

#include <iostream>
#include <SOIL2.h>

namespace OpenGL
{
	using namespace std;

	GLuint Texture::LoadTexture(const char* textureImagePath)
	{
		GLuint textureID;
		textureID = SOIL_load_OGL_texture(textureImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

		if (textureID == 0)
		{
			cout << "could not find texture file" << textureImagePath << endl;
		}

		return textureID;
	}
}