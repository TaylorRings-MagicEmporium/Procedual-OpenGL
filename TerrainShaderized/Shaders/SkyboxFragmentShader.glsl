#version 420 core

out vec4 colourOut;

in vec3 texCoordsExport;

uniform samplerCube skyboxTexture;

void main()
{
	//colourOut = vec4(0,0,1,0);
	colourOut = texture(skyboxTexture, texCoordsExport);
}