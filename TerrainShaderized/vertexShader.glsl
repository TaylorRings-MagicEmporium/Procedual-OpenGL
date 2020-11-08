#version 420 core

layout(location=0) in vec4 terrainCoords;
//layout(location=1) in vec4 terrainColors;
layout(location=1) in vec3 terrainNormals;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;


out vec3 normalExport;



void main(void)
{
	normalExport = normalize(normalMat * terrainNormals);


   gl_Position = projMat * modelViewMat * terrainCoords;
}