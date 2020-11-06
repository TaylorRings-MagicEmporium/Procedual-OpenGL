#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>
//#include <GL/glut.h>
//#include <GL/glext.h>
#pragma comment(lib, "glew32d.lib") 
#pragma comment(lib, "freeglut.lib")

using namespace std;
using namespace glm;

// Size of the terrain
const int MAP_SIZE = 33;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

float terrain[MAP_SIZE][MAP_SIZE] = {};
glm::vec3 CamPos = glm::vec3(-10.0f, -5.0f, -30.0f);
float RANDOM_MAX = 1.0f;
float HEIGHT_MAX = 10.0f;

struct Vertex
{
	float coords[4];
	float colors[4];
};

struct Matrix4x4
{
	float entries[16];
};

static mat4 projMat = mat4(1.0);

static const Matrix4x4 IDENTITY_MATRIX4x4 =
{
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	}
};

static enum buffer { TERRAIN_VERTICES };
static enum object { TERRAIN };

// Globals
static Vertex terrainVertices[MAP_SIZE*MAP_SIZE] = {};

const int numStripsRequired = MAP_SIZE - 1;
const int verticesPerStrip = 2 * MAP_SIZE;

unsigned int terrainIndexData[numStripsRequired][verticesPerStrip];

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
buffer[1],
vao[1];

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
		}
	}

	// TODO: Add your code here to calculate the height values of the terrain using the Diamond square algorithm

	srand(4);

	terrain[0][0] = GetRandom()*HEIGHT_MAX;
	terrain[0][MAP_SIZE - 1] = GetRandom()* HEIGHT_MAX;
	terrain[MAP_SIZE - 1][0] = GetRandom()* HEIGHT_MAX;
	terrain[MAP_SIZE - 1][MAP_SIZE - 1] = GetRandom()* HEIGHT_MAX;

	//for (int i = 0; i < 100; i++) {
	//	GetRandom();
	//}


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
		std::cout << "BEFORE: " << step << std::endl;
		step = step / 2;
		std::cout << "AFTER: " << step << std::endl;
		RANDOM_MAX *= 0.5f;
	}


	//Square_step(step, step, step);

	
	// Intialise vertex array
	int i = 0;

	for (int z = 0; z < MAP_SIZE; z++)
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			// Set the coords (1st 4 elements) and a default colour of black (2nd 4 elements) 
			terrainVertices[i] = { { (float)x, terrain[x][z], (float)z, 1.0 }, { 0.0, 0.0, 0.0, 1.0 } };
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
	
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Create shader program executable - read, compile and link shaders
	char* vertexShader = readTextFile("vertexShader.glsl");
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);

	char* fragmentShader = readTextFile("fragmentShader.glsl");
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);
	///////////////////////////////////////

	// Create vertex array object (VAO) and vertex buffer object (VBO) and associate data with vertex shader.
	glGenVertexArrays(1, vao);
	glGenBuffers(1, buffer);
	glBindVertexArray(vao[TERRAIN]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TERRAIN_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(terrainVertices[0]), (GLvoid*)sizeof(terrainVertices[0].coords));
	glEnableVertexAttribArray(1);
	///////////////////////////////////////

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
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(lol));

	///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// For each row - draw the triangle strip
	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, terrainIndexData[i]);
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
		CamPos += glm::vec3(0, 0, 0.1);
		break;
	case 'a':
		CamPos += glm::vec3(0.1, 0, 0);
		break;
	case 's':
		CamPos += glm::vec3(0, 0, -0.1);
		break;
	case 'd':
		CamPos += glm::vec3(-0.1, 0, 0);
		break;
	default:
		break;
	}
}

void UpdateGame() {

	// Obtain modelview matrix uniform location and set value.
	mat4 modelViewMat = mat4(1.0);
	// Move terrain into view - glm::translate replaces glTranslatef
	modelViewMat = translate(modelViewMat, CamPos); // 5x5 grid
	glm::mat4 lol = lookAt(CamPos, glm::vec3(0), cross(-CamPos, glm::vec3(1, 0, 0)));
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	//glutIdleFunc(UpdateGame);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();

}
