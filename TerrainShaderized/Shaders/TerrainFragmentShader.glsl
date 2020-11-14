#version 420 core

#define TERRAIN 0
#define WATER 1

in vec3 normalExport;
in vec2 texCoords;
in float VertexHeight;
uniform vec4 globAmb;
uniform sampler2D grassTex;
uniform sampler2D sandTex;
//uniform sampler2D rockTex;
//uniform sampler2D snowTex;

uniform float HEIGHT_MAX;

struct Material
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};
struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;

};
uniform Material terrainFandB;
uniform Light light0;

vec3 normal;
vec3 lightDirection;
vec4 AmbDifCombo;
vec4 TexColor;
vec4 tempTexColor;
vec4 total;

float heightPercent;

out vec4 colorsOut;

void main(void)
{

	heightPercent = (VertexHeight + HEIGHT_MAX) / (HEIGHT_MAX*2);

	if(heightPercent < 0.5){
	TexColor = texture(sandTex, texCoords);
	} 
	else {
	TexColor = texture(grassTex, texCoords);
	}

	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	AmbDifCombo = max(dot(normal,lightDirection),0.0f)*(light0.difCols * terrainFandB.difRefl);

	total = TexColor* AmbDifCombo;
	total.w = 1.0;

	colorsOut = total;
}