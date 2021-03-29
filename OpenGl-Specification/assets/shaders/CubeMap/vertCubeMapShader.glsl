#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (binding = 0) uniform samplerCube aCubeMap;

uniform mat4 uModel;		// Model matrix.
uniform mat4 uView;			// View matrix.
uniform mat4 uProjection;	// Projection matrix.

out vec3 TextureCoord;

void main(void)
{
	TextureCoord = aPosition;
	mat4 vrotMatrix = mat4(mat3(uView));
	gl_Position = uProjection * vrotMatrix * vec4(aPosition, 1.0);
}