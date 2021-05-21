#version 440 core

// Vertex shader

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

layout (binding = 0) uniform sampler2D aAlbedoMap;
layout (binding = 1) uniform sampler2D aNormalMap;
layout (binding = 2) uniform sampler2D aMetallicMap;

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

out VS_FS_INTERFACE
{
	//...
} Vertex;

// Fragment shader

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

layout (binding = 0) uniform sampler2D aAlbedoMap;
layout (binding = 1) uniform sampler2D aNormalMap;
layout (binding = 2) uniform sampler2D aMetallicMap;

layout (std140, binding = 24) CameraProperties
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

in VS_FS_INTERFACE
{
	//...
} Fragment;