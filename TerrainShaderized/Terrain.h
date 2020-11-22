#pragma once
#include "GridObjects.h"

enum TextureName { SAND, GRASS, ROCK, SNOW };
class Terrain : public GridObjects
{
private:
	BitMapFile* image[4];
	unsigned int texture[4];
	std::string textureFileNames[4];

	std::vector<glm::vec2> AvailiblePositions;
public:
	Terrain() {};

	void Setup();
	void Close();
	void Update();
	void Draw();
	std::vector<glm::vec4> CreatePositions(float min, float max, int amount);

	void SetImages();
};

