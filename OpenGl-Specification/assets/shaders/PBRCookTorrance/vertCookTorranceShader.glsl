#version 430

struct Material
{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

struct Light
{
	vec3 position;
	vec3 color;
};

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

// PBR maps
layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uMetallicMap;
layout (binding = 2) uniform sampler2D uRoughnessMap;
layout (binding = 3) uniform sampler2D uAOMap;
layout (binding = 4) uniform sampler2D uNormalMap;

// Matrices parameteres
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// Camera parameters
uniform vec3 uCameraPosition;

uniform Material uMaterial;
uniform Light uLight;

out vec3 Position;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;

void main()
{
	Position = (uModel * vec4(aPosition, 1.0)).xyz;
	TexCoord = aTexCoord;
	Normal = aNormal;
	Tangent = aTangent;

	gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}