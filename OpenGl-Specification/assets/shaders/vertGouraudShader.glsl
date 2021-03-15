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

void main(void)
{
	// Convert vertex position to view space.
	vec4 p = uView * uModel * vec4(aPosition, 1.0);

	// Convert normal to view space.
	mat4 normMat = transpose(inverse(uView * uModel));
	vec3 n = normalize((normMat * vec4(aNormal, 1.0)).xyz);

	// Calculate view space light vector (from vertex to light).
	vec3 l = normalize(uLight.position - p.xyz);

	// View vector is equivalent to the negative of view space vertex position.
	vec3 v = normalize(aPosition.xyz);

	// r is reflection of -l with respect to surface normal n
	vec3 r = reflect(-l, n);

	// ambient, diffuse, and specular contributions
	vec3 ambient = ((uGlobalAmbient * uMaterial.ambient) + (uLight.ambient * uMaterial.ambient)).xyz;
	vec3 diffuse = uLight.diffuse.xyz * uMaterial.diffuse.xyz * max(dot(n, l), 0.0);
	vec3 specular = uMaterial.specular.xyz * uLight.specular.xyz * pow(max(dot(r, v), 0.0), uMaterial.shininess);

	// Send the color output to the fragment shader
	VaryingColor = vec4((ambient + diffuse + specular), 1.0);

	// Send the position to the fragment shader
	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}