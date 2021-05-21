#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

layout (binding = 0) uniform sampler2D aAlbedoMap;
layout (binding = 1) uniform sampler2D aAOMap;
layout (binding = 2) uniform sampler2D aHeightMap;
layout (binding = 3) uniform sampler2D aMetallicMap;
layout (binding = 4) uniform sampler2D aNormalMap;
layout (binding = 5) uniform sampler2D aRoughnessMap;
layout (binding = 6) uniform sampler2DShadow aDepthMap;

layout (std140, binding = 24) uniform CameraProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
	vec4 CamDir;
};

layout (std140, binding = 25) uniform LightProperties
{
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightDir;
	vec4 LightColor;
	vec4 LightAmbient;
};

uniform mat4 uModel;
uniform mat4 uShadowMat;
uniform float uTilingFactor;
uniform float uDisplacementFactor;

out VS_FS_INTERFACE
{
	vec4 ShadowCoord;
	vec3 Position;
	vec2 UV;
	mat3 TBN;
} Vertex;

void main()
{

	// Calculate the height map displacing the vertex position
	float displacement = texture(aHeightMap, aUV * uTilingFactor).r * uDisplacementFactor;
	vec3 fixedPosition = aPosition + (aNormal * displacement);
	Vertex.Position = (uModel * vec4(fixedPosition, 1.0)).xyz;

	// Calculate the TBN matrix
	vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(uModel * vec4(aNormal,	0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);	// Retrieve the bitangent vector
	Vertex.TBN = mat3(T, B, N);

	// Calculate uv tiling
	Vertex.UV = aUV * uTilingFactor;

	Vertex.ShadowCoord = uShadowMat * LightProjMat * LightViewMat * vec4(Vertex.Position, 1.0);

	// Output the vertex position in clip space
	gl_Position = CamProjMat * CamViewMat * uModel * vec4(fixedPosition, 1.0);
}