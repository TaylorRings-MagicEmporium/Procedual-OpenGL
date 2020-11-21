#version 420 core


layout (location = 0) in vec3 aPos;

out vec3 texCoordsExport;

uniform mat4 projMat;
uniform mat4 modelViewMat;

void main()
{
	texCoordsExport = aPos;
	gl_Position = projMat * modelViewMat * vec4(aPos,1.0);
}