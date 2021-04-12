#version 430

layout (binding = 0) uniform samplerCube uTexMap;

uniform float uTime;		// current time.
uniform mat4 uModel;		// Model View matrix.
uniform mat4 uView;			// Model View matrix.
uniform mat4 uProjection;	// Projection matrix.

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 Color;

void main(void)
{
	vec3 r = -reflect(normalize(-Position), normalize(Normal));
	Color = texture(uTexMap, r);
}