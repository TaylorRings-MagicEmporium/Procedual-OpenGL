#version 420 core

in vec3 normalExport;
in vec2 texCoords;
in float VertexHeight;
uniform vec4 globAmb;
uniform sampler2D grassTex;
uniform sampler2D sandTex;
uniform sampler2D rockTex;
uniform sampler2D snowTex;

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
int OBJECT = 0;

vec3 normal;
vec3 lightDirection;
vec4 AmbDifCombo;
vec4 TexColor;
vec4 tempTexColor;
vec4 total;

float heightPercent;
float adjust = 0.3;
out vec4 colorsOut;

void main(void)
{

	normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	AmbDifCombo = max(dot(normal,lightDirection),0.0f)*(light0.difCols * terrainFandB.difRefl);


	heightPercent = (VertexHeight + HEIGHT_MAX) / (HEIGHT_MAX*2);

	if(heightPercent <= 0.3){
		TexColor = texture(sandTex, texCoords);
	}
	else if(heightPercent <= 0.35) {
		tempTexColor = texture(sandTex,texCoords);
		TexColor = tempTexColor + (texture(grassTex,texCoords) - tempTexColor) * ((heightPercent - 0.3)/0.05);
	}
	else if(heightPercent <= 0.55){
		TexColor = texture(grassTex, texCoords);
	}
	else if(heightPercent <= 0.6){
		tempTexColor = texture(grassTex,texCoords);
		TexColor = tempTexColor + (texture(rockTex,texCoords) - tempTexColor) * ((heightPercent - 0.55)/0.05);
	}
	else if(heightPercent <= 0.8){
		TexColor = texture(rockTex, texCoords);
	}
	else if(heightPercent <= 0.85){
		tempTexColor = texture(rockTex,texCoords);
		TexColor = tempTexColor + (texture(snowTex,texCoords) - tempTexColor) * ((heightPercent - 0.8)/0.05);
	}
	else{
		TexColor = texture(snowTex, texCoords);
	}

		
	total = TexColor* AmbDifCombo;
	total.w = 1.0;



	colorsOut = total;
}