#version 430

layout (location = 0) in vec3 aPosition;

uniform mat4 uShadowMVP;

void main(void)
{ 
	gl_Position = uShadowMVP * vec4(aPosition, 1.0); 
}