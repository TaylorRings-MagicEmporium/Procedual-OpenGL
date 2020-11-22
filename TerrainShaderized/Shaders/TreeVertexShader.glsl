#version 420 core

#define TRUNK 0
#define LEAF 1

layout(location=0) in vec4 TrunkCoords;
layout(location=1) in vec4 TrunkColors;
layout(location=2) in vec4 LeafCoords;
layout(location=3) in vec4 LeafColors;
layout(location=4) in mat4 Transform;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;
uniform uint object;
uniform float treeFlowRate;
out vec4 colorsExport;

vec4 coords;

void main(void)
{
   if (object == TRUNK) coords = TrunkCoords + vec4(sin(treeFlowRate)/10,0.0,0,0.0);
   if (object == LEAF) coords = LeafCoords + vec4(sin(treeFlowRate)/10,0.0,0,0.0);

   gl_Position = projMat * Transform * coords;
   
   if (object == TRUNK) colorsExport = TrunkColors;  
   if (object == LEAF) colorsExport = LeafColors;  
}