#pragma once
#include "TObjects.h"
// the grass class is responsible of creating small blades of grass for the scene.
// the most basic grass only needs 7 points (or 5 stripped triangles)
class Grass : public TObjects
{
private:
	float blade[21] = {
		0.05,0,0,
		-0.05,0,0,
		0.05,0.2,0,
		-0.05,0.2,0,
		0.05,0.3,0,
		-0.05,0.3,0,
		0,0.4,0
	};

	glm::vec4 positions[10000];
	unsigned int instanceVBO;
public:
	void Setup();
	void Close();
	void Update();
	void Draw();

	void SetVAOVBO();
	void SetPositions(std::vector<glm::vec4> pos);
};

