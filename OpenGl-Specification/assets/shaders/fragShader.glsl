#version 430

in vec2 tc;
in vec3 v_normals;

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{ 
	color = vec4(v_normals * 0.5 + 0.5, 1.0); 
}