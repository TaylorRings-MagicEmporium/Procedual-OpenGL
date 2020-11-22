#version 420 core

in vec3 normalExport;
in vec2 texCoords;
in float VertexHeight;
uniform vec4 globAmb;
uniform sampler2D cloudTex;

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
uniform float waterFlowRate;

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

	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	AmbDifCombo = max(dot(normal,lightDirection),0.0f)*(light0.difCols * terrainFandB.difRefl);

	TexColor = texture(cloudTex, texCoords + vec2(waterFlowRate)/500,0.0);
	
	total = TexColor* AmbDifCombo;
	//total.w = 0.7;

	colorsOut = total;
	//colorsOut = vec4(1,0,1,0);
}