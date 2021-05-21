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

uniform mat4 uModel;
uniform vec4 uColor;

out VS_FS_INTERFACE
{
	vec4 SurfaceColor;
} Vertex;

void main()
{
	Vertex.SurfaceColor = uColor;
	gl_Position = CamProjMat * CamViewMat * uModel * vec4(aPosition, 1.0);
}