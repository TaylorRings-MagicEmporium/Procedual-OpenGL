#pragma once
#include "TObjects.h"

// gridObjects is a base parent that supports components that use grid-like vertices.
class GridObjects : public TObjects
{
protected:

	static const int numStripsRequired = MAP_SIZE - 1;
	static const int verticesPerStrip = 2 * MAP_SIZE;

	Vertex ObjectVertices[MAP_SIZE * MAP_SIZE] = {};
	//float terrain[MAP_SIZE][MAP_SIZE] = {};
	float** objectValue;
	unsigned int IndexData[numStripsRequired][verticesPerStrip];
public:

	float RANDOM_MAX = 1.0f;
	float HEIGHT_MAX = 10.0f;

	void Diamond_step(int x, int y, int step, float** ValueArray);
	void Square_step(int x, int y, int step, float** ValueArray);

	// GetRandom produces a random number between -1 to 1
	float GetRandom();
	void setupVAOVBO();
};

