#version 430

/** 
 * The keyword "in" means "inout" and indicates that this vertex attribute will be receiving values form a buffer.
 * "vec3" means that each invocation of the shader will grab three float values. 
 * The variable name is "position".
 * The "layout (location = 0)" portion of the command is called a "layout qualifier" and is how we will associate the
 * vertex attribute with a paricular buffer.
 */
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormals;

uniform mat4 uTime;			// current time.
uniform mat4 uModel;		// Model matrix.
uniform mat4 uView;			// View matrix.
uniform mat4 uProjection;	// Projection matrix.

out vec3 position;
out vec2 texcoords;
out vec3 normals;

void main(void)
{
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);

	position = aPosition;
	texcoords = aTexCoord;
	normals = aNormals;
}