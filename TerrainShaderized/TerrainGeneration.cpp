#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <chrono>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Terrain.h"
#include "Water.h"
#include "Skybox.h"
#include "Camera.h"
#include "Trees.h"
#include "Grass.h"

using namespace std;
using namespace glm;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


// each instance renders th
Terrain* terrain = new Terrain();
Water* water = new Water();
Skybox* skybox = new Skybox();
Trees* trees = new Trees();
Grass* grass = new Grass();
Camera* camera = new Camera(glm::vec3(-10.0f, -5.0f, -30.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

glm::mat4 projMat;

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



//"Setup" sets up each of the component's properties and provides information like the camera, materials and specific position points.
void setup(void)
{
	projMat = perspective(radians(60.0), (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 1000.0);
	camera->UpdateCamera();

	terrain->Setup();
	terrain->SetCamera(projMat, camera->modelView);
	terrain->SetNormalMat();
	terrain->SetFloat(terrain->HEIGHT_MAX, "HEIGHT_MAX");
	terrain->SetMandL(terrainFandB, light0);

	trees->SetPositions(terrain->CreatePositions(-3.0, 1.0, NUMOFTREES));
	grass->SetPositions(terrain->CreatePositions(-3.0, 3.0, 10000));

	water->Setup();
	water->SetCamera(projMat, camera->modelView);
	water->SetNormalMat();
	water->SetMandL(terrainFandB, light0);

	skybox->Setup();
	skybox->SetCamera(projMat, camera->modelView);
	skybox->SetNormalMat();

	trees->Setup();
	trees->SetCamera(projMat, camera->modelView);
	trees->SetNormalMat();

	grass->Setup();
	grass->SetCamera(projMat, camera->modelView);
	grass->SetNormalMat();
}

// drawScene draws each of the components as well as sorting out what should be enabled. 
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	grass->Draw();
	trees->Draw();


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	skybox->Draw();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	terrain->Draw();
	water->Draw();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glutSwapBuffers();


}

// a function that GLUT uses when the window is resized
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	camera->UpdateScreenSize(w, h);
}

// a function that GLUT uses when a key input was pressed
void keyInput(unsigned char key, int x, int y)
{
	bool update = false;
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'w':
		camera->position += camera->forward * camera->speed;
		update = true;
		break;
	case 's':
		camera->position -= camera->forward * camera->speed;
		update = true;
		break;
	case 'a':
		camera->position += -glm::normalize(glm::cross(camera->forward, glm::vec3(0,1,0))) * camera->speed;
		update = true;
		break;
	case 'd':
		camera->position += glm::normalize(glm::cross(camera->forward, glm::vec3(0, 1, 0))) * camera->speed;
		update = true;
		break;
	default:
		break;
	}

	if (update) {
		camera->UpdateCamera();

	}
}

// a function that GLUT uses for doing the update procedures. it goes through eash of the components and updates the camera with the new modelView
void UpdateGame() {

	//glm::mat4 ViewMat = lookAt(glm::vec3(MAP_SIZE / 2, 10, MAP_SIZE + 10) + CamPos, glm::vec3(MAP_SIZE / 2, 0, MAP_SIZE / 2) + CamPos, cross(glm::normalize(glm::vec3(MAP_SIZE / 2, 10, MAP_SIZE / 2) - CamPos), glm::vec3(1, 0, 0)));

	terrain->SetCamera(projMat, camera->modelView);
	water->SetCamera(projMat, camera->modelView);
	skybox->SetCamera(projMat, camera->modelView);
	trees->SetCamera(projMat, camera->modelView);
	grass->SetCamera(projMat, camera->modelView);
	
	terrain->Update();
	water->Update();
	trees->Update();
	grass->Update();
	glutPostRedisplay();
}

//a function that GLUT uses for detecting mouse movement
void MouseMovement(int x, int y) {
	int dx = x - camera->lastFrameMousePos.x;
	int dy = camera->lastFrameMousePos.y - y;

	camera->RotateCamera(dx, dy);

	camera->lastFrameMousePos = glm::vec2(x, y);
	camera->ResetMousePosition();
}

// the Main function that is run first
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	// Set the version of OpenGL (4.2)
	glutInitContextVersion(4, 2);
	// The core profile excludes all discarded features
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// Forward compatibility excludes features marked for deprecation ensuring compatability with future versions
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_FULL_SCREEN);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("TerrainGeneration");

	glutSetCursor(GLUT_CURSOR_NONE);

	// Set OpenGL to render in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutIdleFunc(UpdateGame);
	glutPassiveMotionFunc(MouseMovement);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();
	glutMainLoop();

	// after everything is done, close all of the components.
	water->Close();
	terrain->Close();
	skybox->Close();
	grass->Close();
	trees->Close();
	delete water;
	delete terrain;
	delete skybox;
	delete trees;
	delete grass;
}

