#pragma once
#include "TObjects.h"

enum object {TRUNK,LEAF};
enum buffer {TRUNK_VERTICES, LEAF_VERTICES};
static const int NUMPOINTS = 1000;

struct TVertex
{
	float coords[4];
	float colors[4];
};


class Trees : public TObjects
{
private:
	unsigned int leafVAO,
		leafVBO,
	objectLoc;

	//const int NUMPOINTS = 1000;
	const int MAXLEVEL = 6;
	const float R = 0.85;
	const float alfa = 40.0;
	TVertex TrunkVertices[NUMPOINTS] = {};
	TVertex LeafVertices[NUMPOINTS] = {};
public:
	Trees() {};
	void ComputeSingleBranch(float angle, float x0, float y0, float x1, float y1, float& x, float& y);
	void RecurComputeBranch(int depth, int index, float angle, std::vector<glm::vec2> basePts, vector<glm::vec2> BrPts);
	void Setup();
	void Close();
	void Update();
	void Draw();
	void CreateVAOVBO();

};

