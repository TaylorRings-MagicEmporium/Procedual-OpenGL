#version 420 core

#define TERRAIN 0
#define WATER 1

in vec3 normalExport;
in vec2 texCoords;
in float VertexHeight;
in float OBJECT_VAL;
uniform vec4 globAmb;
uniform sampler2D grassTex;
uniform sampler2D sandTex;
uniform sampler2D waterTex;
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



	if(OBJECT_VAL == WATER){

		TexColor = texture(waterTex, texCoords);

		total = vec4(1.0,0.0,0.0,0.5);
		total.w = 0.5;

	} else{

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
	}


	
	colorsOut = total;

//
//	if(heightPercent <= 60){
//		TexColor = texture(sandTex, texCoords);
//	}
//	else if(heightPercent <= 65) {
//		tempTexColor = texture(sandTex,texCoords);
//		TexColor = tempTexColor + (texture(grassTex,texCoords) - tempTexColor) * ((heightPercent - 60)/5);
//	}
//	else if(heightPercent <= 75){
//		TexColor = texture(grassTex, texCoords);
//	}
//	else if(heightPercent <= 80){
//		tempTexColor = texture(grassTex,texCoords);
//		TexColor = tempTexColor + (texture(rockTex,texCoords) - tempTexColor) * ((heightPercent - 75)/5);
//	}
//	else if(heightPercent <= 90){
//		TexColor = texture(rockTex, texCoords);
//	}
//	else if(heightPercent <= 95){
//		tempTexColor = texture(rockTex,texCoords);
//		TexColor = tempTexColor + (texture(snowTex,texCoords) - tempTexColor) * ((heightPercent - 90)/5);
//	}
//	else{
//		TexColor = texture(snowTex, texCoords);
//	}



//   normal = normalize(normalExport);
//   lightDirection = normalize(vec3(light0.coords));
//   //TexColor = texture(grassTex, texCoords);
//   AmbDifCombo = max(dot(normal,lightDirection),0.0f)*(light0.difCols * terrainFandB.difRefl);
//   total = TexColor* AmbDifCombo;
//   colorsOut = vec4(total.x,total.y,total.z,1.0);
//   //colorsOut = AmbDifCombo;
}