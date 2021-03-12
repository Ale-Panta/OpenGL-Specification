#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;

out vec2 tc;	// Texture coordinates output to rasterizer for interpolation
out vec3 v_normals;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
	tc = texCoord;
	v_normals = normals;
}

mat4 buildTranslate(float x, float y, float z)
{
	mat4 translation = mat4
	(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0
	);

	return translation;
}

mat4 buildRotateX(float rad)
{
	mat4 rot = mat4
	(
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(rad), -sin(rad), 0.0,
		0.0, sin(rad), cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return rot;
}

mat4 buildRotateY(float rad)
{
	mat4 rot = mat4
	(
		cos(rad), 0.0, sin(rad), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rad), 0.0, cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return rot;
}

mat4 buildRotateZ(float rad)
{
	mat4 rot = mat4
	(
		cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	return rot;
}