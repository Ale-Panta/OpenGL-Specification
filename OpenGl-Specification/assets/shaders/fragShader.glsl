#version 430

in vec3 position;
in vec3 normals;

out vec4 color;

void main(void)
{ 
	color = vec4(normals * 0.5 + 0.5, 1.0); 
}