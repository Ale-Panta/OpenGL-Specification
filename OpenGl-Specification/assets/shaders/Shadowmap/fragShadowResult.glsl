#version 440 core

layout (binding = 0) uniform sampler2D aMap;

in vec2 TexCoord;

out vec4 color;

void main()
{
	color = vec4(vec3(texture(aMap, TexCoord).r), 1.0);
}