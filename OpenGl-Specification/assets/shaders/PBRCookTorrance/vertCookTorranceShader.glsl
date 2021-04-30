#version 430

// --- Begin layout vertex data ---------------------------------------------------------------------------------------

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

// --- End layout vertex data -----------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin layout uniform textures ----------------------------------------------------------------------------------

layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uAOMap;
layout (binding = 2) uniform sampler2D uHeightMap;
layout (binding = 3) uniform sampler2D uMetallicMap;
layout (binding = 4) uniform sampler2D uNormalMap;
layout (binding = 5) uniform sampler2D uRoughnessMap;

// --- End layout uniform textures ------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin layout uniform blocks ------------------------------------------------------------------------------------

layout (std140, binding = 24) uniform CameraProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
};

layout (std140, binding = 25) uniform LightProperties
{
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin local program uniforms -----------------------------------------------------------------------------------

uniform mat4 uModel;
uniform float uTilingFactor;
uniform float uDisplacementFactor;

// --- End local program uniforms -------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin out values -----------------------------------------------------------------------------------------------

out vec3 Position;
out vec2 UV;
out mat3 TBN;

// --- End out values -------------------------------------------------------------------------------------------------

void main()
{
	// Calculate the height map displacing the vertex position

	float displacement = texture(uHeightMap, aUV * uTilingFactor).r * uDisplacementFactor;
	vec3 fixedPosition = aPosition + (aNormal * displacement);
	Position = (uModel * vec4(fixedPosition, 1.0)).xyz;

	// Calculate the TBN matrix

	vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(uModel * vec4(aNormal,	0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);	// Retrieve the bitangent vector
	TBN = mat3(T, B, N);

	// Calculate uv tiling

	UV = aUV * uTilingFactor;

	// Output the vertex position in clip space

	gl_Position = CamProjMat * CamViewMat * uModel * vec4(fixedPosition, 1.0);
}