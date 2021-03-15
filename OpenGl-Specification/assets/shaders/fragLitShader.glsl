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

out vec4 Color;

void main(void)
{ 
	Color = VaryingColor; 
}