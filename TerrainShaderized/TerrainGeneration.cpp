#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include <vector>
#include <chrono>

#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <GL/glut.h>
//#include <GL/glext.h>
#pragma comment(lib, "glew32d.lib") 
#pragma comment(lib, "freeglut.lib")

#include "getbmp.h"

using namespace std;
using namespace glm;

// Size of the terrain
const int MAP_SIZE = 33;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

float terrain[MAP_SIZE][MAP_SIZE] = {};
float water[MAP_SIZE][MAP_SIZE] = {};
glm::vec3 CamPos = glm::vec3(-10.0f, -5.0f, -30.0f);
float RANDOM_MAX = 1.0f;
float HEIGHT_MAX = 8.0f;


auto startTime = std::chrono::system_clock::now();
float waterHightDivider = 3.0f;

struct Vertex
{
	vec4 coords;
	//vec4 colors;
	vec3 normal;
	vec2 texcoords;
};

struct Matrix4x4
{
	float entries[16];
};

struct Material 
{
	vec4 ambRefl;
	vec4 difRefl;
	vec4 specRefl;
	vec4 emitCols;
	float shininess;
};

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec4 coords;

};

static const Material terrainFandB =
{
	vec4(1.0f,1.0f,1.0f,1.0f),
	vec4(1.0f,1.0f,1.0f,1.0f),
	vec4(1.0f,1.0f,1.0f,1.0f),
	vec4(0.0f,0.0f,0.0f,1.0f),
	50.0f

};

static const Light light0 =
{
	vec4(0.0,0.0,0.0,1.0),
	vec4(1.0,1.0,1.0,1.0),
	vec4(1.0,1.0,1.0,1.0),
	vec4(1.0,1.0,0.0,1.0)

};

static mat4 projMat = mat4(1.0);
static mat3 normalMat = mat3(1.0);

static BitMapFile* image[5];

static unsigned int
texture[5],
TexLoc[5];

//static const Matrix4x4 IDENTITY_MATRIX4x4 =
//{
//	{
//		1.0, 0.0, 0.0, 0.0,
//		0.0, 1.0, 0.0, 0.0,
//		0.0, 0.0, 1.0, 0.0,
//		0.0, 0.0, 0.0, 1.0
//	}
//};
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

static enum buffer { TERRAIN_VERTICES, WATER_VERTICES };
static enum object { TERRAIN, WATEROBJ };
static enum TextureName { SAND, GRASS, ROCK, SNOW, WATER };

// Globals
static Vertex terrainVertices[MAP_SIZE*MAP_SIZE] = {};
static Vertex waterVertices[MAP_SIZE * MAP_SIZE] = {};

const int numStripsRequired = MAP_SIZE - 1;
const int verticesPerStrip = 2 * MAP_SIZE;

unsigned int terrainIndexData[numStripsRequired][verticesPerStrip];
unsigned int waterIndexData[numStripsRequired][verticesPerStrip];

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
buffer[2],
vao[2];

float GetRandom() {
	float r = (((rand() % 10) / 5.0f) - 1.0f);
	//std::cout << r << std::endl;
	return r;

}

void Diamond_step(int x, int y, int step) {
	float p1, p2, p3, p4;
	p1 = terrain[x][y];
	p2 = terrain[x][y+step];
	p3 = terrain[x+step][y];
	p4 = terrain[x+step][y+step];
	//step = step / 2;

	terrain[x + step / 2][y + step / 2] = ((p1 + p2 + p3 + p4) / 4.0f) +GetRandom() * HEIGHT_MAX * RANDOM_MAX;
}

void Square_step(int x, int y, int step) {

	float p5, p6, p7, p8;
	float count = 0;
	//left
	p5 = terrain[x][y];
	p6 = terrain[x][y + step];
	count += 2;
	if (x - step / 2 < 0) {
		p7 = 0.0f;
	}
	else {
		p7 = terrain[x - step / 2][y + step / 2];
		count += 1;
	}
	if (x + step / 2 > MAP_SIZE) {
		p8 = 0.0f;
	}
	else {
		p8 = terrain[x + step / 2][y + step / 2];
		count += 1;
	}

	terrain[x][y + step / 2] = ((p5 + p6 + p7 + p8) / count) +GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;


	//up
	p5 = terrain[x][y];
	p6 = terrain[x + step][y];
	count += 2;
	if (y - step / 2 < 0) {
		p7 = 0.0f;
	}
	else {
		p7 = terrain[x + step / 2][y - step / 2];
		count += 1;
	}
	if (y + step / 2 > MAP_SIZE) {
		p8 = 0.0f;
	}
	else {
		p8 = terrain[x + step / 2][y + step / 2];
		count += 1;
	}

	terrain[x + step / 2][y] = ((p5 + p6 + p7 + p8) / count) +GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;
	//right
	p5 = terrain[x+step][y];
	p6 = terrain[x+step][y + step];
	p7 = terrain[x + step / 2][y + step / 2];
	count += 3;
	//if (x + step / 2 > MAP_SIZE ) {
	//	p7 = 0.0f;
	//}
	//else {
	//	p7 = terrain[x + step / 2][y + step / 2];
	//	count += 1;
	//}
	if (x + ((step / 2) * 3) > MAP_SIZE) {
		p8 = 0.0f;
	}
	else {
		p8 = terrain[x + ((step / 2) * 3)][y + step / 2];
		count += 1;
	}

	terrain[x + step][y + step / 2] = ((p5 + p6 + p7 + p8) / count) +GetRandom() * HEIGHT_MAX * RANDOM_MAX;

	count = 0;
	//down
	p5 = terrain[x][y+step];
	p6 = terrain[x+step][y + step];
	p7 = terrain[x + step / 2][y + step / 2];
	count += 3;
	//if (x + step / 2 > MAP_SIZE || y + step / 2 > MAP_SIZE) {
	//	p7 = 0.0f;
	//}
	//else {

	//	count += 1;
	//}
	if (y + ((step / 2) * 3) > MAP_SIZE) {
		p8 = 0.0f;
	}
	else {
		p8 = terrain[x + step / 2][y + ((step / 2) * 3)];
		count += 1;
	}

	terrain[x + step / 2][y + step] = ((p5 + p6 + p7 + p8) / count) +GetRandom() * HEIGHT_MAX * RANDOM_MAX;

}

// Function to read text file, used to read shader files
char* readTextFile(char* aTextFile)
{
	FILE* filePointer = fopen(aTextFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void shaderCompileTest(GLuint shader) {
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

// Initialization routine.
void setup(void)
{
	// Initialise terrain - set values in the height map to 0
	//float terrain[MAP_SIZE][MAP_SIZE] = {};
	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int z = 0; z < MAP_SIZE; z++)
		{
			terrain[x][z] = 0;
			water[x][z] = 0;
		}
	}

	// TODO: Add your code here to calculate the height values of the terrain using the Diamond square algorithm

	srand(2);

	terrain[0][0] = GetRandom()*HEIGHT_MAX;
	terrain[0][MAP_SIZE - 1] = GetRandom()* HEIGHT_MAX;
	terrain[MAP_SIZE - 1][0] = GetRandom()* HEIGHT_MAX;
	terrain[MAP_SIZE - 1][MAP_SIZE - 1] = GetRandom()* HEIGHT_MAX;

	int step = MAP_SIZE - 1;
	while (step > 1) {
		for (int x = 0; x < MAP_SIZE-1; x += step) {
			for (int y = 0; y < MAP_SIZE-1; y += step) {
				Diamond_step(x, y, step);
			}
		}

		for (int x = 0; x < MAP_SIZE-1; x += step) {
			for (int y = 0; y < MAP_SIZE-1; y += step) {
				//std::cout << x << ", " << y << std::endl;
				Square_step(x, y, step);
			}
		}
		//std::cout << std::endl;
		//std::cout << "BEFORE: " << step << std::endl;
		step = step / 2;
		//std::cout << "AFTER: " << step << std::endl;
		RANDOM_MAX *= 0.5f;
	}
	
	// Intialise vertex array
	int i = 0;

	//calculate texture coords
	float fTexS = float(MAP_SIZE) * 0.1f;
	float fTexT = float(MAP_SIZE) * 0.1f;

	for (int z = 0; z < MAP_SIZE; z++)
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			float fScaleS = float(x) / float(MAP_SIZE - 1);
			float fScaleT = float(z) / float(MAP_SIZE - 1);

			terrainVertices[i] = {
				vec4((float)x,terrain[x][z],(float)z,1.0),
				//vec4(0.0,0.0,0.0,1.0),
				vec3(0.0,1.0,0.0),
				vec2((1.0f / (float(MAP_SIZE) - 1)) * x,(1.0f / (float(MAP_SIZE) - 1)) * z)
				//vec2(fTexS * fScaleS,fTexT * fScaleT)
			};

			waterVertices[i] = {
				vec4((float)x,water[x][z],(float)z, 1.0),
				vec3(0.0, 1.0, 0.0),
				vec2((1.0f / (float(MAP_SIZE) - 1)) * x, (1.0f / (float(MAP_SIZE) - 1)) * z)
			};

			std::cout << (1.0f / (float(MAP_SIZE)-1)) * (x) << ", " << (1.0f / (float(MAP_SIZE)-1)) * (z) << std::endl;
			i++;
		}
	}

	// Now build the index data 
	i = 0;
	for (int z = 0; z < MAP_SIZE - 1; z++)
	{
		i = z * MAP_SIZE;
		for (int x = 0; x < MAP_SIZE * 2; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			terrainIndexData[z][x] = i;
			i++;
		}
	}
	i = 0;
	for (int z = 0; z < MAP_SIZE - 1; z++)
	{
		i = z * MAP_SIZE;
		for (int x = 0; x < MAP_SIZE * 2; x += 2)
		{
			waterIndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			waterIndexData[z][x] = i;
			i++;
		}
	}

	//computing normal vectors
	int index1, index2, index3;
	vec3 p1, p2, p3;
	vec3 edge1, edge2;
	vec3 crossNorm, finalNorm;
	for (int z = 0; z < MAP_SIZE - 1; z++) {
		for (int x = 0; x < (MAP_SIZE * 2 - 2); x++) {
			index1 = terrainIndexData[z][x];
			index2 = terrainIndexData[z][x+1];
			index3 = terrainIndexData[z][x+2];

			p1 = vec3(terrainVertices[index1].coords);
			p2 = vec3(terrainVertices[index2].coords);
			p3 = vec3(terrainVertices[index3].coords);

			edge1 = p2 - p1;
			edge2 = p3 - p1;

			//crossNorm = cross(edge2, edge1);
			if (x % 2 == 1) {
				crossNorm = cross(edge2, edge1);
			}
			else {
				crossNorm = cross(edge1, edge2);
			}
			if (dot(crossNorm, vec3(0, 1, 0)) < 1e-06f) {
				finalNorm = -crossNorm;
			}
			else {
				finalNorm = crossNorm;
			}

			//finalNorm = crossNorm;

			terrainVertices[index1].normal = finalNorm + terrainVertices[index1].normal;
			terrainVertices[index2].normal = finalNorm + terrainVertices[index2].normal;
			terrainVertices[index3].normal = finalNorm + terrainVertices[index3].normal;

		}
	}

	//normalising normals
	for (int i = 0; i < (MAP_SIZE * MAP_SIZE) - 1; i++) {
		terrainVertices->normal = normalize(terrainVertices->normal);
	}
	
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable - read, compile and link shaders
	char* vertexShader = readTextFile("Shaders/TerrainVertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	std::cout << "VERTEX::" << std::endl;
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile("Shaders/TerrainFragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	std::cout << "FRAGMENT::" << std::endl;
	shaderCompileTest(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	///////////////////////////////////////

	// Create vertex array object (VAO) and vertex buffer object (VBO) and associate data with vertex shader.

	glUseProgram(programId);

	glGenVertexArrays(2, vao);
	glGenBuffers(2, buffer);

	glBindVertexArray(vao[TERRAIN]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TERRAIN_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)sizeof(terrainVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)(sizeof(terrainVertices[0].normal) + sizeof(terrainVertices[0].coords)));
	glEnableVertexAttribArray(2);
	/////////////////////////////////////////

	// Obtain projection matrix uniform location and set value.
	projMatLoc = glGetUniformLocation(programId, "projMat");
	projMat = perspective(radians(60.0), (double) SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100.0);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	///////////////////////////////////////

	// Obtain modelview matrix uniform location and set value.
	mat4 modelViewMat = mat4(1.0);
	// Move terrain into view - glm::translate replaces glTranslatef
	modelViewMat = translate(modelViewMat, CamPos); // 5x5 grid
	glm::mat4 lol = lookAt(glm::vec3(MAP_SIZE/2,10,MAP_SIZE + 10), glm::vec3(MAP_SIZE/2,0,MAP_SIZE/2), cross(glm::normalize(glm::vec3(MAP_SIZE/2,0,MAP_SIZE/2) - CamPos), glm::vec3(1, 0, 0)));
	//glm::mat4 lol = lookAt(glm::vec3(0,0,0), glm::vec3(MAP_SIZE / 2, 0, MAP_SIZE / 2), cross(glm::normalize(glm::vec3(MAP_SIZE / 2, 0, MAP_SIZE / 2) - CamPos), glm::vec3(1, 0, 0)));
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(lol));

	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.ambRefl"), 1, &terrainFandB.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.difRefl"), 1, &terrainFandB.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.specRefl"), 1, &terrainFandB.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programId, "terrainFandB.emitCols"), 1, &terrainFandB.emitCols[0]);
	glUniform1f(glGetUniformLocation(programId, "terrainFandB.shininess"), terrainFandB.shininess);

	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
	glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

	///////////////////////////////////////

	unsigned int normalMatLoc = glGetUniformLocation(programId, "normalMat");
	normalMat = transpose(inverse(mat3(modelViewMat)));
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));

	unsigned int Loc = glGetUniformLocation(programId, "HEIGHT_MAX");
	glUniform1f(Loc, HEIGHT_MAX);

	///////////////////////////////////////

	//load textures
	image[GRASS] = getbmp("Textures/grass.bmp");

	glGenTextures(5, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[GRASS]->sizeX, image[GRASS]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[GRASS]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc[GRASS] = glGetUniformLocation(programId, "grassTex");
	glUniform1i(TexLoc[GRASS], 0);

	//glBindTexture(GL_TEXTURE_2D, 0);
	//////////////////////////////////////

	image[SAND] = getbmp("Textures/sand.bmp");

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[SAND]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[SAND]->sizeX, image[SAND]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[SAND]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc[SAND] = glGetUniformLocation(programId, "sandTex");
	glUniform1i(TexLoc[SAND], 1);

	image[WATER] = getbmp("Textures/water.bmp");

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture[WATER]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[WATER]->sizeX, image[WATER]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[WATER]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc[WATER] = glGetUniformLocation(programId, "waterTex");
	glUniform1i(TexLoc[WATER], 2);

	//////////////////////////////////////

	glBindVertexArray(0);

	//////////////////////////////////////
	glBindVertexArray(vao[object::WATEROBJ]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[WATER_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), (GLvoid*)sizeof(waterVertices[0].coords));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(waterVertices[0]), (GLvoid*)(sizeof(waterVertices[0].normal) + sizeof(waterVertices[0].coords)));
	glEnableVertexAttribArray(2);

}



// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glUseProgram(programId);

	unsigned int Loc = glGetUniformLocation(programId, "OBJECT");
	glUniform1i(Loc, 0);
	glBindVertexArray(vao[TERRAIN]);
	//For each row - draw the triangle strip
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, terrainIndexData[i]);
	}

	glUniform1i(Loc, 1);

	glBindVertexArray(vao[object::WATEROBJ]);
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, waterIndexData[i]);
	}

	glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		CamPos += glm::vec3(0, 0, -0.1);
		break;
	case 'a':
		CamPos += glm::vec3(-0.1, 0, 0);
		break;
	case 's':
		CamPos += glm::vec3(0, 0, 0.1);
		break;
	case 'd':
		CamPos += glm::vec3(0.1, 0, 0);
		break;
	default:
		break;
	}
}

void UpdateGame() {

	//// Obtain modelview matrix uniform location and set value.
	//mat4 modelViewMat = mat4(1.0);
	// Move terrain into view - glm::translate replaces glTranslatef
	//modelViewMat = translate(modelViewMat, CamPos); // 5x5 grid

	glm::mat4 lol = lookAt(glm::vec3(MAP_SIZE / 2, 10, MAP_SIZE + 10) + CamPos, glm::vec3(MAP_SIZE / 2, 0, MAP_SIZE / 2) + CamPos, cross(glm::normalize(glm::vec3(MAP_SIZE / 2, 10, MAP_SIZE / 2) - CamPos), glm::vec3(1, 0, 0)));

	glUseProgram(programId);

	auto endTime = std::chrono::system_clock::now();
	auto dist = endTime - startTime;
	float ti = dist.count() / 1e7f;
	
	unsigned int Loc = glGetUniformLocation(programId, "waterFlowRate");
	glUniform1f(Loc, ti);

	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(lol));

	glutPostRedisplay();
}

// Main routine.
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	// Set the version of OpenGL (4.2)
	glutInitContextVersion(4, 2);
	// The core profile excludes all discarded features
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// Forward compatibility excludes features marked for deprecation ensuring compatability with future versions
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TerrainGeneration");

	// Set OpenGL to render in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutIdleFunc(UpdateGame);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();

}
