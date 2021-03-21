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
layout (location = 3) in vec3 aColor;

layout (binding = 0) uniform sampler2DShadow aShadowTex;

struct Material {
	vec3 globalAmbient;
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

uniform float uTime;		// current time.
uniform mat4 uModel;		// Model View matrix.
uniform mat4 uView;			// Model View matrix.
uniform mat4 uProjection;	// Projection matrix.

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in vec3 VertexColor;
in vec3 LightDir;
in vec3 HalfVector;
in vec4 ShadowCoord;

out vec4 Color;

void main(void)
{ 
	// Normalize the light, normal and view vectors
	vec3 l = normalize(LightDir.xyz);
	vec3 n = normalize(Normal.xyz);
	vec3 p = normalize(-Position.xyz);
	vec3 h = normalize(HalfVector);

	// Get the angle between the light and surface normal
	float cosTheta = dot(l, n);
	
	// Angle between the view vector and reflected light
	float cosPhi = dot(h, n);
	
	float notInShadow = textureProj(aShadowTex, ShadowCoord);

	Color = vec4(uMaterial.globalAmbient * uMaterial.ambient + uLight.ambient * uMaterial.ambient, 1.0);
	
	if (notInShadow == 1.0)
	{
		// Compute ADS contributions (per pixel), and combine to build output color
		vec3 ambient = ((uMaterial.globalAmbient * uMaterial.ambient) + (uLight.ambient * uMaterial.ambient)).xyz;
		vec3 diffuse = uLight.diffuse.xyz * uMaterial.diffuse.xyz * max(cosTheta, 0.0);
		vec3 specular = uLight.specular.xyz * uMaterial.specular.xyz * pow(max(cosPhi, 0.0), uMaterial.shininess * 3.0);

		Color += vec4((ambient + diffuse + specular), 1.0);
	}
}