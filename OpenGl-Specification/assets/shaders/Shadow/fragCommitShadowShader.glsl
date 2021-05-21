#version 440 core

layout (location = 0) out vec4 aColor;

void main()
{
	aColor = vec4(vec3(gl_FragCoord.z), 1.0);
}