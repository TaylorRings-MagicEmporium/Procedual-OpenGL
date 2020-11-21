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

	std::vector<glm::vec3> positions;

	//const int NUMPOINTS = 1000;
	const int MAXLEVEL = 6;
	const float R = 0.85;
	const float alfa = 40.0;
	TVertex TrunkVertices[NUMPOINTS] = {};
	TVertex LeafVertices[NUMPOINTS] = {};
public:
	Trees() {};
	void ComputeSingleBranch(int depth, float angle, glm::vec3 start, glm::vec3 end, glm::vec3& returnData);
	void RecurComputeBranch(int depth, int index, float angle, std::vector<glm::vec3> basePts, vector<glm::vec3> BrPts);
	void Setup();
	void Close();
	void Update();
	void Draw();
	void SetupPosition();
	void CreateVAOVBO();

};

