#version 430

/** 
 * The keyword "in" means "inout" and indicates that this vertex attribute will be receiving values form a buffer.
 * "vec3" means that each invocation of the shader will grab three float values. 
 * The variable name is "position".
 * The "layout (location = 0)" portion of the command is called a "layout qualifier" and is how we will associate the
 * vertex attribute with a paricular buffer.
 */
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_textCoord;
layout (location = 2) in vec3 v_normals;

uniform mat4 u_time;	// current time.
uniform mat4 u_mvMat;	// Model View matrix.
uniform mat4 u_pMat;	// Projection matrix.

out vec3 position;
out vec2 texcoords;
out vec3 normals;

void main(void)
{
	gl_Position = u_pMat * u_mvMat * vec4(v_position, 1.0);
	position = v_position;
	normals = v_normals;
	texcoords = v_textCoord;
}