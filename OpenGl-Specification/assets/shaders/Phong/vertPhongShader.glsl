#version 430

/**
 * The structure of the algorithm is similar to the algorithm for Gouraud shading, except that the lighting computations
 * are done per pixel rather than per vertex. Since the lighting computations require a normal vector N ans a light vector
 * L, which are only available in the model on a per vertex basis, Phong shading is often impemented using a clever trick, 
 * whereby N and L are computed in the vertex shader and interpolated during rasterization.
 *
 */

/** 
 * The keyword "in" means "inout" and indicates that this vertex attribute will be receiving values form a buffer.
 * "vec3" means that each invocation of the shader will grab three float values. 
 * The variable name is "position".
 * The "layout (location = 0)" portion of the command is called a "layout qualifier" and is how we will associate the
 * vertex attribute with a paricular buffer.
 */
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 uGlobalAmbient;
uniform Material uMaterial;
uniform Light uLight;

uniform mat4 uTime;			// current time.
uniform mat4 uModel;		// Model matrix.
uniform mat4 uView;			// View matrix.
uniform mat4 uProjection;	// Projection matrix.

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;
out vec4 VaryingColor;

out vec3 LightDir;

void main(void)
{
	// Output vertex position, light direction, and normal to the rasterizer for interpolation.

	Position = (uView * uModel * vec4(aPosition, 1.0)).xyz;
	LightDir = uLight.position.xyz - Position.xyz;
	
	mat4 normMat = transpose(inverse(uView * uModel));
	Normal = (normMat * vec4(aNormal, 1.0)).xyz;

	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}