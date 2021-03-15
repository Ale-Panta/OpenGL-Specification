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
out vec3 HalfVector;

void main(void)
{
	// Output vertex position, light direction, and normal to the rasterizer for interpolation.

	Position = (uView * uModel * vec4(aPosition, 1.0)).xyz;
	LightDir = uLight.position.xyz - Position.xyz;
	HalfVector = (LightDir + (-Position)).xyz;

	mat4 normMat = transpose(inverse(uView * uModel));
	Normal = (normMat * vec4(aNormal, 1.0)).xyz;

	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}