#version 440 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;

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
uniform vec4 uColor;

out VS_FS_INTERFACE
{
	vec4 SurfaceColor;
	vec3 FragPosition;
	vec3 FragNormal;
} Vertex;

void main()
{
	Vertex.SurfaceColor = uColor;

    Vertex.FragPosition = (uModel * vec4(aPosition, 1.0)).xyz;
    Vertex.FragNormal = mat3(uModel * CamViewMat) * aNormal;

	gl_Position = CamProjMat * CamViewMat * uModel * vec4(aPosition, 1.0);
}