#include "Camera.h"


void Camera::UpdateCamera() {
	if (pitch < -89) {
		pitch = -89;
	}
	if (pitch > 89) {
		pitch = 89;
	}
	glm::vec3 eye = glm::vec3(
		glm::cos(glm::radians(pitch)) * -glm::cos(glm::radians(yaw)),
		glm::sin(glm::radians(pitch)),
		glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw)));


	forward = glm::normalize(eye);

	modelView = glm::lookAt(forward, glm::vec3(0), glm::vec3(0, 1, 0));
	modelView = glm::translate(modelView, position);
}

void Camera::ResetMousePosition() {
	if (lastFrameMousePos.x < 200 || lastFrameMousePos.x > SCREEN_WIDTH - 200 ||
		lastFrameMousePos.y < 200 || lastFrameMousePos.y > SCREEN_HEIGHT - 200)
	{
		glutWarpPointer(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		lastFrameMousePos.x = SCREEN_WIDTH / 2;
		lastFrameMousePos.y = SCREEN_HEIGHT / 2;
	}
}

void Camera::UpdateScreenSize(int w, int h) {
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
}

void Camera::RotateCamera(int x, int y) {
	if (x != 0) {
		yaw += x < 0 ? 1.0f : -1.0f;
	}

	if (y != 0) {
		pitch += y < 0 ? 1.0f : -1.0f;
	}

	UpdateCamera();
}