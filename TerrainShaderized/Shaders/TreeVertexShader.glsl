#version 420 core

#define TRUNK 0
#define LEAF 1

layout(location=0) in vec4 TrunkCoords;
layout(location=1) in vec4 TrunkColors;
layout(location=2) in vec4 LeafCoords;
layout(location=3) in vec4 LeafColors;
layout(location=4) in vec4 PosOffset;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;
uniform uint object;

out vec4 colorsExport;

vec4 coords;

void main(void)
{
   if (object == TRUNK) coords = (TrunkCoords * 0.05) + PosOffset;
   if (object == LEAF) coords = (LeafCoords * 0.05) + PosOffset;

   gl_Position = projMat * modelViewMat * coords;
   
   if (object == TRUNK) colorsExport = TrunkColors;  
   if (object == LEAF) colorsExport = LeafColors;  
}