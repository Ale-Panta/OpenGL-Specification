#version 440 core

layout (binding = 0) uniform sampler2D aShadowmap;

in VS_FS_INTERFACE
{
	vec2 UV;
} Fragment;

out vec4 Color;

void main()
{
	Color = vec4(vec3(texture(aShadowmap, Fragment.UV).r), 1.0);
}