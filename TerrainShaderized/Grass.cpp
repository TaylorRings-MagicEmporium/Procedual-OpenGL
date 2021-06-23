#include "Grass.h"

void Grass::Setup() {
	CreateShader("Shaders/GrassVertexShader.glsl", "Shaders/GrassFragmentShader.glsl");
	SetVAOVBO();

}

void Grass::Close()
{
}

void Grass::Update()
{
}

void Grass::Draw() {
	glUseProgram(programID);
	glBindVertexArray(vao);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 7);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 7, 10000);
	glBindVertexArray(0);
}

void Grass::SetVAOVBO() {
	glUseProgram(programID);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &buffer);
	glGenBuffers(1, &instanceVBO);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blade), blade, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Grass::SetPositions(std::vector < glm::vec4> pos) {
	for (int i = 0; i < 10000; i++) {
		pos[i] += glm::vec4(((float)rand() / (float)RAND_MAX)/2.0,0, ((float)rand() / (float)RAND_MAX)/2.0,0);
	}
	std::copy(pos.begin(), pos.end(), positions);
}