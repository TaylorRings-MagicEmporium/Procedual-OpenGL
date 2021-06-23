#pragma once
#include "GridObjects.h"

#include <ctime>

// the water class is responsible in controlling the water flow, size and height
class Water : public GridObjects
{
private:
	BitMapFile* image[1];
	unsigned int texture[1];
	std::string textureFileNames[1];
	using clock_t = std::chrono::system_clock;
	std::chrono::time_point<clock_t> beginTime;
public:
	Water() { beginTime = clock_t::now(); };

	void Setup();
	void Close();
	void Update();
	void Draw();

	void SetImages();
};

