#version 430

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

uniform float uTime;		// current time.
uniform mat4 uModel;		// Model View matrix.
uniform mat4 uView;			// Model View matrix.
uniform mat4 uProjection;	// Projection matrix.

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in vec4 VaryingColor;

in vec3 LightDir;
in vec3 HalfVector;

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
	
	// Compute ADS contributions (per pixel), and combine to build output color
	vec3 ambient = ((uGlobalAmbient * uMaterial.ambient) + (uLight.ambient * uMaterial.ambient)).xyz;
	vec3 diffuse = uLight.diffuse.xyz * uMaterial.diffuse.xyz * max(cosTheta, 0.0);
	vec3 specular = uLight.specular.xyz * uMaterial.specular.xyz * pow(max(cosPhi, 0.0), uMaterial.shininess * 3.0);

	Color = vec4((ambient + diffuse + specular), 1.0);
}