#version 420 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 offset;

uniform mat4 projMat;
uniform mat4 modelViewMat;

void main()
{
	gl_Position = projMat * modelViewMat * (vec4(aPos,0.0) + offset + vec4(0,-0.03,0,0));
}