#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

layout (binding = 0) uniform samplerCube uTexMap;

uniform mat4 uTime;			// current time.
uniform mat4 uModel;		// Model matrix.
uniform mat4 uView;			// View matrix.
uniform mat4 uProjection;	// Projection matrix.

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;

void main(void)
{
	// Output vertex position, light direction, and normal to the rasterizer for interpolation.

	Position = (uModel * vec4(aPosition, 1.0)).xyz;
	mat4 normMat = transpose(inverse(uModel));
	Normal = (normMat * vec4(aNormal, 1.0)).xyz;

	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}