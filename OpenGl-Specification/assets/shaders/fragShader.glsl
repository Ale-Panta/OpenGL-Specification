#version 430

in vec3 position;
in vec2 texcoords;
in vec3 normals;

out vec4 color;

uniform float uTime;		// current time.
uniform mat4 uModel;		// Model View matrix.
uniform mat4 uView;			// Model View matrix.
uniform mat4 uProjection;	// Projection matrix.

void main(void)
{ 
	color = vec4(1.0); 
}