#version 420 core

layout(location=0) in vec4 terrainCoords;
//layout(location=1) in vec4 terrainColors;
layout(location=1) in vec3 terrainNormals;
layout(location=2) in vec2 terrainTexCoods;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;


out vec3 normalExport;
out vec2 texCoords;


void main(void)
{
	normalExport = normalize(normalMat * terrainNormals);

	texCoords = terrainTexCoods;
   gl_Position = projMat * modelViewMat * terrainCoords;
}