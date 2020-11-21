#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "getbmp.h"
#include "SOIL/SOIL.h"

static const int MAP_SIZE = 65;

struct Vertex
{
	glm::vec4 coords;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct Material
{
	glm::vec4 ambRefl;
	glm::vec4 difRefl;
	glm::vec4 specRefl;
	glm::vec4 emitCols;
	float shininess;
};

struct Light
{
	glm::vec4 ambCols;
	glm::vec4 difCols;
	glm::vec4 specCols;
	glm::vec4 coords;
};

class TObjects
{
protected:
	glm::vec4 globAmb = glm::vec4(0.2, 0.2, 0.2, 1.0);

	unsigned int 
		programID,
		buffer,
		vao;

public:
	virtual void Setup() = 0;
	virtual void Close() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void shaderCompileTest(GLuint shader);
	char* readTextFile(char* aTextFile);

	void CreateShader(std::string vertexShad, std::string fragShad);

	void SetCamera(glm::mat4 projMat, glm::mat4 modelView);
	void SetMandL(Material m, Light l);

	void SetFloat(float da, std::string name);
	void SetVec3(glm::vec3 da, std::string name);
	void SetVec4(glm::vec4 da, std::string name);
};

