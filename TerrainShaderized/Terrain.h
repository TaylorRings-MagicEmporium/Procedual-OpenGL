#pragma once
#include "GridObjects.h"

enum TextureName { SAND, GRASS, ROCK, SNOW };
class Terrain : public GridObjects
{
private:
	BitMapFile* image[4];
	unsigned int texture[4];
	std::string textureFileNames[4];


public:
	Terrain() {};

	void Setup();
	void Close();
	void Update();
	void Draw();

	void SetImages();
};

