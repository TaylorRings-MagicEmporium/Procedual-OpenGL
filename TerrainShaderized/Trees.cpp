#include "Trees.h"

void Trees::ComputeSingleBranch(float angle, float x0, float y0, float x1, float y1, float &x, float &y) {
	float xs, ys, xl, yl;

	xs = (x1 - x0) * R; //R is the length factor for branches
	ys = (y1 - y0) * R;

	xl = cos(angle / 2.0) * xs - sin(angle / 2.0) * ys;
	yl = sin(angle / 2.0) * xs + cos(angle / 2.0) * ys;

	x = x1 + xl;
	y = y1 + yl;
}

void Trees::RecurComputeBranch(int depth, int index, float angle, std::vector<glm::vec2> basePts, vector<glm::vec2> BrPts) {
	int i, size;
	float x2, y2;
	glm::vec2 ttPt;
	vector<glm::vec2> NewBasePts, NewBrPts;

	if (depth > MAXLEVEL) return;

	size = basePts.size();
	if (size == 0) return;
	std::cout << index << std::endl;
	for (i = 0; i < size; i++)
	{
		ComputeSingleBranch(angle, basePts[i].x,basePts[i].y, BrPts[i].x, BrPts[i].y, x2, y2);
		TrunkVertices[index].coords[0] = x2;
		TrunkVertices[index].coords[1] = y2;
		TrunkVertices[index].coords[2] = 0.0;
		TrunkVertices[index].coords[3] = 1.0;
		index++;
		NewBasePts.push_back(BrPts[i]);
		ttPt.x = x2; ttPt.y = y2;
		NewBrPts.push_back(ttPt);

		ComputeSingleBranch(-angle, basePts[i].x, basePts[i].y, BrPts[i].x, BrPts[i].y, x2, y2);
		TrunkVertices[index].coords[0] = x2;
		TrunkVertices[index].coords[1] = y2;
		TrunkVertices[index].coords[2] = 0.0;
		TrunkVertices[index].coords[3] = 1.0;
		index++;
		NewBasePts.push_back(BrPts[i]);
		ttPt.x = x2; ttPt.y = y2;
		NewBrPts.push_back(ttPt);
	}

	//newx0 = x1;
	//newy0 = y1;
	depth++;
	RecurComputeBranch(depth, index, angle, NewBasePts, NewBrPts);
}

void Trees::Setup()
{
	int i, count;
	float x0, y0, x1, y1, x2, y2, xs, ys, xl, yl, angle;
	angle = 65 * 3.1415926 / 180.0;
	glm::vec2 ttPt;
	std::vector<glm::vec2> BasePts, BranchPts;

	for (i = 0; i < NUMPOINTS; i++) {
		//TrunkVertices[i].normal = glm::vec3(0, 1, 0);
		//TrunkVertices[i].texcoords = glm::vec2(0);
		TrunkVertices[i].colors[0] = 0.55;
		TrunkVertices[i].colors[1] = 0.27;
		TrunkVertices[i].colors[2] = 0.075;
		TrunkVertices[i].colors[3] = 1.0;
	}

	//main trunk
	TrunkVertices[0].coords[0] = 0.0;
	TrunkVertices[0].coords[1] = 0.0;
	TrunkVertices[0].coords[2] = 0.0;
	TrunkVertices[0].coords[3] = 1.0;
	ttPt.x = TrunkVertices[0].coords[0];
	ttPt.y = TrunkVertices[0].coords[1];
	BasePts.push_back(ttPt);

	TrunkVertices[1].coords[0] = 0.0;
	TrunkVertices[1].coords[1] = 15.0;
	TrunkVertices[1].coords[2] = 0.0;
	TrunkVertices[1].coords[3] = 1.0;
	ttPt.x = TrunkVertices[1].coords[0];
	ttPt.y = TrunkVertices[1].coords[1];
	BranchPts.push_back(ttPt);

	//compute the rest of the branches
	i = 2;
	RecurComputeBranch(0, i, angle, BasePts, BranchPts);

	int totalPts, leafPts;
	totalPts = pow(2, MAXLEVEL) - 2;
	leafPts = pow(2, MAXLEVEL - 1);
	for (i = 0; i < NUMPOINTS; i++) {
		LeafVertices[i].colors[0] = 0.0;
		LeafVertices[i].colors[1] = 1.0;
		LeafVertices[i].colors[2] = 0.0;
		LeafVertices[i].colors[3] = 1.0;
	}

	count = 0;
	for (i = totalPts; i > (totalPts - leafPts); i--) {
		ttPt.x = TrunkVertices[i].coords[0];
		ttPt.y = TrunkVertices[i].coords[1];

		LeafVertices[count].coords[0] = ttPt.x;
		LeafVertices[count].coords[1] = ttPt.y;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1.732;
		LeafVertices[count].coords[1] = ttPt.y + 1;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1;
		LeafVertices[count].coords[1] = ttPt.y + 1.732;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		////second one
		LeafVertices[count].coords[0] = ttPt.x + 1.732;
		LeafVertices[count].coords[1] = ttPt.y + 1;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1;
		LeafVertices[count].coords[1] = ttPt.y + 1.732;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 2.732;
		LeafVertices[count].coords[1] = ttPt.y + 2.732;
		LeafVertices[count].coords[2] = 0.0;
		LeafVertices[count].coords[3] = 1.0;
		count++;
	}

	CreateShader("Shaders/TreeVertexShader.glsl", "Shaders/TreeFragmentShader.glsl");
	CreateVAOVBO();
	objectLoc = glGetUniformLocation(programID, "object");
}

void Trees::Close()
{

}

void Trees::Update()
{

}

void Trees::Draw()
{
	glUseProgram(programID);
	const int BRANCH_INDEX_COUNT = 254;
	glm::uint branchIndexData[BRANCH_INDEX_COUNT];
	int index, size, firstone;
	vector<int> BrArr, ttArr;
	branchIndexData[0] = 0; branchIndexData[1] = 1;
	index = 2;
	BrArr.push_back(1);
	firstone = 1;
	for (int i = 0; i < (MAXLEVEL - 1); i++) {
		size = BrArr.size();
		for (int j = 0; j < size; j++) ttArr.push_back(BrArr[j]);
		BrArr.clear();
		for (int j = 0; j < size; j++) {
			branchIndexData[index] = ttArr[j];
			index++;
			branchIndexData[index] = ttArr[j] * 2;
			index++;
			BrArr.push_back(ttArr[j] * 2);

			branchIndexData[index] = ttArr[j];
			index++;
			branchIndexData[index] = ttArr[j] * 2 + 1;
			index++;

			BrArr.push_back(ttArr[j] * 2 + 1);
		}
		ttArr.clear();
	}

	glUniform1ui(objectLoc, TRUNK);
	glBindVertexArray(vao);


	glLineWidth(0.5f);
	glDrawElements(GL_LINES, BRANCH_INDEX_COUNT, GL_UNSIGNED_INT, branchIndexData);
	glLineWidth(5.0f);
	glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 2, GL_UNSIGNED_INT, branchIndexData);
	glLineWidth(4.0f);
	glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 2 + 4, GL_UNSIGNED_INT, branchIndexData);
	glLineWidth(3.0f);
	glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 6 + 8, GL_UNSIGNED_INT, branchIndexData);
	glLineWidth(2.0f);
	glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 14 + 16, GL_UNSIGNED_INT, branchIndexData);
	glLineWidth(1.0f);
	glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 30 + 32, GL_UNSIGNED_INT, branchIndexData);

	int leafNum, leafVerNum;
	leafNum = pow(2, MAXLEVEL - 1);
	leafVerNum = leafNum * 6;
	glUniform1ui(objectLoc, LEAF); // Draw Trunk
	glBindVertexArray(leafVAO);
	glDrawArrays(GL_TRIANGLES, 0, leafVerNum); //leafVerNum
}

void Trees::CreateVAOVBO()
{
	glUseProgram(programID);
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &leafVAO);

	glGenBuffers(1, &buffer);
	glGenBuffers(1, &leafVBO);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TrunkVertices), TrunkVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TrunkVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TrunkVertices[0]), (GLvoid*)sizeof(TrunkVertices[0].coords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(leafVAO);
	glBindBuffer(GL_ARRAY_BUFFER, leafVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LeafVertices), LeafVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(LeafVertices[0]), 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(LeafVertices[0]), (GLvoid*)sizeof(LeafVertices[0].coords));
	glEnableVertexAttribArray(3);

}
