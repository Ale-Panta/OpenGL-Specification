#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (binding = 0) uniform samplerCube aCubeMap;

uniform mat4 uModel;		// Model matrix.
uniform mat4 uView;			// View matrix.
uniform mat4 uProjection;	// Projection matrix.

in vec3 TextureCoord;

out vec4 Color;

void main(void)
{
	Color = texture(aCubeMap, TextureCoord);
}