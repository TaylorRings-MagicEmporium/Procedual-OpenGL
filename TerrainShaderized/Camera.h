#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>

// the camera class is responsible of moving and rotating the camera based on the user's input.
class Camera
{
public:
	glm::vec3 position = glm::vec3(0);
	glm::vec3 forward = glm::vec3(0);
	float yaw = 90.0;
	float pitch = 0;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	glm::vec2 lastFrameMousePos;
	
	glm::mat4 modelView;

	float speed = 0.1;
	
	Camera(glm::vec3 pos, int w, int h) { this->position = pos; SCREEN_WIDTH = w; SCREEN_HEIGHT = h; };
	void UpdateCamera();
	void UpdateScreenSize(int w, int h);
	void RotateCamera(int x, int y);
	void ResetMousePosition();
};

