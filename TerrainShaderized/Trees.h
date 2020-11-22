#pragma once
#include "TObjects.h"

enum object {TRUNK,LEAF};
enum buffer {TRUNK_VERTICES, LEAF_VERTICES};
static const int NUMPOINTS = 1000;
static const int NUMOFTREES = 1000;

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
		instanceVBO,
	objectLoc;

	glm::vec4 positions[NUMOFTREES];

	//const int NUMPOINTS = 1000;
	const int MAXLEVEL = 6;
	const float R = 0.85;
	const float alfa = 40.0;
	TVertex TrunkVertices[NUMPOINTS] = {};
	TVertex LeafVertices[NUMPOINTS] = {};

	using clock_t = std::chrono::system_clock;
	std::chrono::time_point<clock_t> beginTime;
public:
	Trees() { beginTime = clock_t::now(); };
	void ComputeSingleBranch(int depth, float angle, glm::vec3 start, glm::vec3 end, glm::vec3& returnData);
	void RecurComputeBranch(int depth, int index, float angle, std::vector<glm::vec3> basePts, vector<glm::vec3> BrPts);
	void Setup();
	void Close();
	void Update();
	void Draw();
	void SetupPosition();
	void CreateVAOVBO();

	void SetPositions(std::vector<glm::vec4> pos);

};

