#include "GridObjects.h"

float GridObjects::GetRandom() {
	float r = (((rand() % 10) / 5.0f) - 1.0f);
	//std::cout << r << std::endl;
	return r;

}

void GridObjects::Diamond_step(int x, int y, int step, float** ValueArray) {

	//int (*ptr)[MAP_SIZE] = *ValueArray;
	float p1, p2, p3, p4;
	p1 = ValueArray[x][y];
	p2 = ValueArray[x][y + step];
	p3 = ValueArray[x + step][y];
	p4 = ValueArray[x + step][y + step];
	//step = step / 2;

	ValueArray[x + step / 2][y + step / 2] = ((p1 + p2 + p3 + p4) / 4.0f) + GetRandom() * HEIGHT_MAX * RANDOM_MAX;
}

void GridObjects::Square_step(int x, int y, int step, float** ValueArray) {

	float p5, p6, p7, p8;
	float count = 0;
	//left
	p5 = ValueArray[x][y];
	p6 = ValueArray[x][y + step];
	count += 2;
	if (x - step / 2 < 0) {
		p7 = 0.0f;
	}
	else {
		p7 = ValueArray[x - step / 2][y + step / 2];
		count += 1;
	}
	if (x + step / 2 > MAP_SIZE - 1) {
		p8 = 0.0f;
	}
	else {
		p8 = ValueArray[x + step / 2][y + step / 2];
		count += 1;
	}

	ValueArray[x][y + step / 2] = ((p5 + p6 + p7 + p8) / count) + GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;


	//up
	p5 = ValueArray[x][y];
	p6 = ValueArray[x + step][y];
	count += 2;
	if (y - step / 2 < 0) {
		p7 = 0.0f;
	}
	else {
		p7 = ValueArray[x + step / 2][y - step / 2];
		count += 1;
	}
	if (y + step / 2 > MAP_SIZE - 1) {
		p8 = 0.0f;
	}
	else {
		p8 = ValueArray[x + step / 2][y + step / 2];
		count += 1;
	}

	ValueArray[x + step / 2][y] = ((p5 + p6 + p7 + p8) / count) + GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;
	//right
	p5 = ValueArray[x + step][y];
	p6 = ValueArray[x + step][y + step];
	p7 = ValueArray[x + step / 2][y + step / 2];
	count += 3;

	if (x + ((step / 2) * 3) > MAP_SIZE - 1) {
		p8 = 0.0f;
	}
	else {
		//std::cout << x + ((step / 2) * 3) << ", " << y + step / 2 << std::endl;
		p8 = ValueArray[x + ((step / 2) * 3)][y + step / 2];
		count += 1;
	}

	ValueArray[x + step][y + step / 2] = ((p5 + p6 + p7 + p8) / count) + GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;
	//down
	p5 = ValueArray[x][y + step];
	p6 = ValueArray[x + step][y + step];
	p7 = ValueArray[x + step / 2][y + step / 2];
	count += 3;

	if (y + ((step / 2) * 3) > MAP_SIZE - 1) {
		p8 = 0.0f;
	}
	else {
		p8 = ValueArray[x + step / 2][y + ((step / 2) * 3)];
		count += 1;
	}

	ValueArray[x + step / 2][y + step] = ((p5 + p6 + p7 + p8) / count) + GetRandom() * HEIGHT_MAX * RANDOM_MAX;

}

void GridObjects::setupVAOVBO() {
	glUseProgram(programID);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &buffer);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ObjectVertices), ObjectVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ObjectVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectVertices[0]), (GLvoid*)sizeof(ObjectVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ObjectVertices[0]), (GLvoid*)(sizeof(ObjectVertices[0].normal) + sizeof(ObjectVertices[0].coords)));
	glEnableVertexAttribArray(2);
}
