#version 440 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

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

out VS_FS_INTERFACE
{
	vec4 ShadowCoord;
	vec3 WorldCoord;
	vec3 EyeCoord;
	vec3 Normal;
} Vertex;

void main()
{
	vec4 worldPos = uModel * vec4(aPosition, 1.0);
	vec4 eyePos = CamViewMat * worldPos;
	vec4 clipPos = CamProjMat * eyePos;

	Vertex.WorldCoord = worldPos.xyz;
	Vertex.EyeCoord = eyePos.xyz;
	Vertex.ShadowCoord = uShadowMat * LightProjMat * LightViewMat * worldPos;
	Vertex.Normal = mat3(CamViewMat * uModel) * aNormal;

	gl_Position = clipPos;
}