#version 430

in vec3 position;
in vec2 texcoords;
in vec3 normals;

out vec4 color;

uniform float u_time;	// current time.
uniform mat4 u_mvMat;	// Model View matrix.
uniform mat4 u_pMat;	// Projection matrix.

void main(void)
{ 
	color = vec4(texcoords, 0.0, 1.0); 
}