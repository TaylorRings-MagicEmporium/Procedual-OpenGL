#include "Trees.h"

void Trees::ComputeSingleBranch(int depth, float angle, glm::vec3 start, glm::vec3 end, glm::vec3& returnData) {
	glm::vec3 s,ll,l;
	float m;
	double val;


	s.x = (end.x - start.x) * R; //R is the length factor for branches
	s.y = (end.y - start.y) * R;
	s.z = (end.z - start.z) * R;
	
	//m = glm::sqrt(s.x * s.x + s.y * s.y + s.z * s.z);

	ll.x = cos(angle / 2.0) * s.x - sin(angle / 2.0) * s.y;
	ll.y = sin(angle / 2.0) * s.x + cos(angle / 2.0) * s.y;
	ll.z = 0.0;

	if (depth % 2 == 0) {
		val = -2;
	}
	else {
		val = 2;
	}

	l.x = cos(val * angle / 2.0) * ll.x - sin(val * angle / 2.0) * ll.z;
	l.y = ll.y;
	l.z = cos(val * angle / 2.0) * ll.x + sin(val * angle / 2.0) * ll.z;

	returnData.x = end.x + l.x;
	returnData.y = end.y + l.y;
	returnData.z = end.z + l.z;
}

void Trees::RecurComputeBranch(int depth, int index, float angle, std::vector<glm::vec3> basePts, vector<glm::vec3> BrPts) {
	int i, size;
	glm::vec3 returnedData;
	glm::vec3 ttPt;
	vector<glm::vec3> NewBasePts, NewBrPts;

	if (depth > MAXLEVEL) return;

	size = basePts.size();
	if (size == 0) return;
	std::cout << index << std::endl;
	for (i = 0; i < size; i++)
	{
		ComputeSingleBranch(depth, angle, glm::vec3(basePts[i].x,basePts[i].y,basePts[i].z), glm::vec3(BrPts[i].x, BrPts[i].y, BrPts[i].z), returnedData);
		TrunkVertices[index].coords[0] = returnedData.x;
		TrunkVertices[index].coords[1] = returnedData.y;
		TrunkVertices[index].coords[2] = returnedData.z;
		TrunkVertices[index].coords[3] = 1.0;
		index++;
		NewBasePts.push_back(BrPts[i]);
		ttPt.x = returnedData.x; ttPt.y = returnedData.y;
		NewBrPts.push_back(ttPt);

		ComputeSingleBranch(depth, -angle, glm::vec3(basePts[i].x, basePts[i].y, basePts[i].z), glm::vec3(BrPts[i].x, BrPts[i].y, BrPts[i].z), returnedData);
		TrunkVertices[index].coords[0] = returnedData.x;
		TrunkVertices[index].coords[1] = returnedData.y;
		TrunkVertices[index].coords[2] = returnedData.z;
		TrunkVertices[index].coords[3] = 1.0;
		index++;
		NewBasePts.push_back(BrPts[i]);
		ttPt.x = returnedData.x; ttPt.y = returnedData.y;
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
	angle = 40.0 * 3.1415926 / 180.0;
	glm::vec3 ttPt;
	std::vector<glm::vec3> BasePts, BranchPts;

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
	ttPt.z = TrunkVertices[0].coords[2];
	BasePts.push_back(ttPt);

	TrunkVertices[1].coords[0] = 0.0;
	TrunkVertices[1].coords[1] = 15;
	TrunkVertices[1].coords[2] = 0.0;
	TrunkVertices[1].coords[3] = 1.0;
	ttPt.x = TrunkVertices[1].coords[0];
	ttPt.y = TrunkVertices[1].coords[1];
	ttPt.z = TrunkVertices[1].coords[2];
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
		ttPt.z = TrunkVertices[i].coords[2];

		LeafVertices[count].coords[0] = ttPt.x;
		LeafVertices[count].coords[1] = ttPt.y;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1.732;
		LeafVertices[count].coords[1] = ttPt.y + 1;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1;
		LeafVertices[count].coords[1] = ttPt.y + 1.732;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		////second one
		LeafVertices[count].coords[0] = ttPt.x + 1.732;
		LeafVertices[count].coords[1] = ttPt.y + 1;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 1;
		LeafVertices[count].coords[1] = ttPt.y + 1.732;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;

		LeafVertices[count].coords[0] = ttPt.x + 2.732;
		LeafVertices[count].coords[1] = ttPt.y + 2.732;
		LeafVertices[count].coords[2] = ttPt.z;
		LeafVertices[count].coords[3] = 1.0;
		count++;
	}

	CreateShader("Shaders/TreeVertexShader.glsl", "Shaders/TreeFragmentShader.glsl");
	CreateVAOVBO();
	objectLoc = glGetUniformLocation(programID, "object");
	SetupPosition();
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

	glm::uint L1[2], L2[6], L3[14], L4[30], L5[62], L6[BRANCH_INDEX_COUNT - 62];
	memcpy(L1, branchIndexData, 2 * sizeof(glm::uint));
	memcpy(L2, branchIndexData, 6 * sizeof(glm::uint));
	memcpy(L3, branchIndexData, 14 * sizeof(glm::uint));
	memcpy(L4, branchIndexData, 30 * sizeof(glm::uint));
	memcpy(L5, branchIndexData, 62 * sizeof(glm::uint));
	memcpy(L6, branchIndexData, (BRANCH_INDEX_COUNT - 62) * sizeof(glm::uint));

	for (int i = 0; i < positions.size(); i++) {
		glUniform1ui(objectLoc, TRUNK);
		glBindVertexArray(vao);

		glm::mat4 temp = glm::mat4(1);
		//temp = modelView;
		temp = glm::translate(modelView, positions[i]);
		temp = glm::scale(temp, glm::vec3(0.05, 0.05, 0.05));
		unsigned int modelViewMatLoc = glGetUniformLocation(programID, "modelViewMat");
		glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(temp));


		
		//glLineWidth(1.0f);
		//glDrawElements(GL_LINES, BRANCH_INDEX_COUNT, GL_UNSIGNED_INT, branchIndexData);
		glLineWidth(10.0f);
		//glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 2, GL_UNSIGNED_INT, branchIndexData);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, L1);
		glLineWidth(8.0f);
		//glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 2 + 4, GL_UNSIGNED_INT, branchIndexData);
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, L2);
		glLineWidth(6.0f);
		//glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 6 + 8, GL_UNSIGNED_INT, branchIndexData);
		glDrawElements(GL_LINES, 14, GL_UNSIGNED_INT, L3);
		glLineWidth(4.0f);
		//glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 14 + 16, GL_UNSIGNED_INT, branchIndexData);
		glDrawElements(GL_LINES, 30, GL_UNSIGNED_INT, L4);
		glLineWidth(2.0f);
		//glDrawRangeElements(GL_LINES, 0, BRANCH_INDEX_COUNT, 30 + 32, GL_UNSIGNED_INT, branchIndexData);
		glDrawElements(GL_LINES, 62, GL_UNSIGNED_INT, L5);
		glLineWidth(1.0f);
		glDrawElements(GL_LINES, BRANCH_INDEX_COUNT - 62, GL_UNSIGNED_INT, L6);

		int leafNum, leafVerNum;
		leafNum = pow(2, MAXLEVEL - 1);
		leafVerNum = leafNum * 6;
		glUniform1ui(objectLoc, LEAF); // Draw Trunk
		glBindVertexArray(leafVAO);
		glDrawArrays(GL_TRIANGLES, 0, leafVerNum); //leafVerNum
	}



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

void Trees::SetupPosition() {
	for (int i = 0; i < 100; i++) {
		positions.push_back(glm::vec3(rand() % MAP_SIZE, 0, rand() % MAP_SIZE));
	}
}