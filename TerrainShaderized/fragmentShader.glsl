#version 420 core

in vec3 normalExport;
uniform vec4 globAmb;

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


out vec4 colorsOut;

void main(void)
{
   normal = normalize(normalExport);
   lightDirection = normalize(vec3(light0.coords));
   colorsOut = max(dot(normal,lightDirection),0.0f)*(light0.difCols * terrainFandB.difRefl);
}