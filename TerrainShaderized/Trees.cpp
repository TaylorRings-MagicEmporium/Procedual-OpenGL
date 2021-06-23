#include "Trees.h"

// creates an angle based on the previous angle and position to create a branch
void Trees::ComputeSingleBranch(int depth, float angle, glm::vec3 start, glm::vec3 end, glm::vec3& returnData) {
	glm::vec3 s,ll,l;
	float m;
	double val;


	s.x = (end.x - start.x) * R; //R is the length factor for branches based on the size of the parent branch
	s.y = (end.y - start.y) * R;
	s.z = (end.z - start.z) * R;
	
	// calculates the 2D angles of the branch
	ll.x = cos(angle / 2.0) * s.x - sin(angle / 2.0) * s.y;
	ll.y = sin(angle / 2.0) * s.x + cos(angle / 2.0) * s.y;
	ll.z = 0.0;

	if (depth % 2 == 0) {
		val = -2;
	}
	else {
		val = 2;
	}

	// calculates the 3D angles of the branch
	l.x = cos(val * angle / 2.0) * ll.x - sin(val * angle / 2.0) * ll.z;
	l.y = ll.y;
	l.z = cos(val * angle / 2.0) * ll.x + sin(val * angle / 2.0) * ll.z;

	returnData.x = end.x + l.x;
	returnData.y = end.y + l.y;
	returnData.z = end.z + l.z;
}

// a recursive function that produces branches of various sizes
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
		// this process is repeated twice for two branches from each parent.
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

	// assigns the colour for the branches
	for (i = 0; i < NUMPOINTS; i++) {
		//TrunkVertices[i].normal = glm::vec3(0, 1, 0);
		//TrunkVertices[i].texcoords = glm::vec2(0);
		TrunkVertices[i].colors[0] = 0.55;
		TrunkVertices[i].colors[1] = 0.27;
		TrunkVertices[i].colors[2] = 0.075;
		TrunkVertices[i].colors[3] = 1.0;
	}

	// produces a "branch" that acts as the trunk
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
	FillMatArray();
	//SetupPosition();
	CreateVAOVBO();
	objectLoc = glGetUniformLocation(programID, "object");

}

void Trees::Close()
{

}

void Trees::Update()
{
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<float> d = endTime - beginTime;
	float ti = d.count();
	//std::cout << ti << std::endl;

	SetFloat(ti, "treeFlowRate");

	FillMatArray();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, NUMOFTREES * sizeof(glm::mat4), objectTransform);

	glBindVertexArray(leafVAO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, NUMOFTREES * sizeof(glm::mat4), objectTransform);
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

	// instead of element range, the branch index count is seperated into defined sizes
	glm::uint L1[2], L2[6], L3[14], L4[30], L5[62], L6[BRANCH_INDEX_COUNT - 62];
	memcpy(L1, branchIndexData, 2 * sizeof(glm::uint));
	memcpy(L2, branchIndexData, 6 * sizeof(glm::uint));
	memcpy(L3, branchIndexData, 14 * sizeof(glm::uint));
	memcpy(L4, branchIndexData, 30 * sizeof(glm::uint));
	memcpy(L5, branchIndexData, 62 * sizeof(glm::uint));
	memcpy(L6, branchIndexData, (BRANCH_INDEX_COUNT - 62) * sizeof(glm::uint));

	glUniform1ui(objectLoc, TRUNK);
	glBindVertexArray(vao);

	unsigned int modelViewMatLoc = glGetUniformLocation(programID, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelView));

	glLineWidth(10.0f);
	glDrawElementsInstanced(GL_LINES, 2, GL_UNSIGNED_INT, L1, NUMOFTREES);
	glLineWidth(8.0f);
	glDrawElementsInstanced(GL_LINES, 6, GL_UNSIGNED_INT, L2, NUMOFTREES);
	glLineWidth(6.0f);
	glDrawElementsInstanced(GL_LINES, 14, GL_UNSIGNED_INT, L3, NUMOFTREES);
	glLineWidth(4.0f);
	glDrawElementsInstanced(GL_LINES, 30, GL_UNSIGNED_INT, L4, NUMOFTREES);
	glLineWidth(2.0f);
	glDrawElementsInstanced(GL_LINES, 62, GL_UNSIGNED_INT, L5, NUMOFTREES);
	glLineWidth(1.0f);
	glDrawElementsInstanced(GL_LINES, BRANCH_INDEX_COUNT - 62, GL_UNSIGNED_INT, L6, NUMOFTREES);

	glUniform1ui(objectLoc, LEAF);
	glBindVertexArray(leafVAO);

	int leafNum, leafVerNum;
	leafNum = pow(2, MAXLEVEL - 1);
	leafVerNum = leafNum * 6;
	glUniform1ui(objectLoc, LEAF); // Draw Trunk
	glBindVertexArray(leafVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, leafVerNum, NUMOFTREES); //leafVerNum
}

void Trees::CreateVAOVBO()
{
	glUseProgram(programID);
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &leafVAO);
	

	glGenBuffers(1, &buffer);
	glGenBuffers(1, &leafVBO);
	glGenBuffers(1, &instanceVBO);


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TrunkVertices), TrunkVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TrunkVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TrunkVertices[0]), (GLvoid*)sizeof(TrunkVertices[0].coords));
	glEnableVertexAttribArray(1);

	// the reason why the instance VBO has 4 locations is for the 4 vectors of a 4x4 matrix. shaders can only accept up to vector4 at a time.
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(objectTransform), objectTransform, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4*sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);

	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);


	glBindVertexArray(leafVAO);
	glBindBuffer(GL_ARRAY_BUFFER, leafVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LeafVertices), LeafVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(LeafVertices[0]), 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(LeafVertices[0]), (GLvoid*)sizeof(LeafVertices[0].coords));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(objectTransform), objectTransform, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4* sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 *sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);

	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 *sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Trees::SetupPosition() {
	for (int i = 0; i < NUMOFTREES; i++) {
		positions[i] = glm::vec4(rand() % MAP_SIZE, 0, rand() % MAP_SIZE,1.0);
		objectAngle[i] = rand() % 90;
	}
}

void Trees::SetPositions(std::vector < glm::vec4> pos) {
	std::copy(pos.begin(), pos.end(), positions);
	for (int i = 0; i < NUMOFTREES; i++) {
		objectAngle[i] = rand() % 90;
	}
}

// creates a array of matrices that are fed directly to the shader using locations
void Trees::FillMatArray() {
	for (int i = 0; i < NUMOFTREES; i++) {
		glm::mat4 temp = modelView;
		temp = glm::translate(temp, glm::vec3(positions[i]) + glm::vec3(0,-0.1,0));
		temp = glm::rotate(temp, objectAngle[i], glm::vec3(0, 1, 0));
		temp = glm::scale(temp, glm::vec3(0.05, 0.05, 0.05));

		objectTransform[i] = temp;
	}
}