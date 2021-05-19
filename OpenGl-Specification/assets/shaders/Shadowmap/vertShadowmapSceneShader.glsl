#version 440 core

// --- Begin layout vertex data ---------------------------------------------------------------------------------------

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;

// --- End layout vertex data -----------------------------------------------------------------------------------------
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
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin local program uniforms -----------------------------------------------------------------------------------

uniform mat4 uModel;
uniform mat4 uShadowMat;

// --- End local program uniforms -------------------------------------------------------------------------------------

out vec4 ShadowCoord;
out vec3 WorldCoord;
out vec3 EyeCoord;
out vec3 Normal;

void main()
{
	vec4 worldPos = uModel * vec4(aPosition, 1.0);
	vec4 eyePos = CamViewMat * worldPos;
	vec4 clipPos = CamProjMat * eyePos;

	WorldCoord = worldPos.xyz;
	EyeCoord = eyePos.xyz;
	ShadowCoord = uShadowMat * LightProjMat * LightViewMat * worldPos;
	Normal = mat3(CamViewMat * uModel) * aNormal;

	gl_Position = clipPos;
}