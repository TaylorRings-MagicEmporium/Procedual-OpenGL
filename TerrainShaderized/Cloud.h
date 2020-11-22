#pragma once
#include "GridObjects.h"
class Cloud : public GridObjects
{
private:

	unsigned int texture[1];

	float* data  = new float[MAP_SIZE*MAP_SIZE*4];

public:
	Cloud() {};
	void Setup();
	void Close();
	void Update();
	void Draw();

	void SetImage();
};

