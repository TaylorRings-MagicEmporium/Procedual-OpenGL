#include "Cloud.h"

void Cloud::Setup()
{
	objectValue = new float* [MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; i++) objectValue[i] = new float[MAP_SIZE];

	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int z = 0; z < MAP_SIZE; z++)
		{
			objectValue[x][z] = 0;
		}
	}

	srand(3);

	objectValue[0][0] = GetRandom() * HEIGHT_MAX;
	objectValue[0][MAP_SIZE - 1] = GetRandom() * HEIGHT_MAX;
	objectValue[MAP_SIZE - 1][0] = GetRandom() * HEIGHT_MAX;
	objectValue[MAP_SIZE - 1][MAP_SIZE - 1] = GetRandom() * HEIGHT_MAX;

	int step = MAP_SIZE - 1;
	while (step > 1) {
		for (int x = 0; x < MAP_SIZE - 1; x += step) {
			for (int y = 0; y < MAP_SIZE - 1; y += step) {
				Diamond_step(x, y, step, objectValue);
			}
		}

		for (int x = 0; x < MAP_SIZE - 1; x += step) {
			for (int y = 0; y < MAP_SIZE - 1; y += step) {
				//std::cout << x << ", " << y << std::endl;
				Square_step(x, y, step, objectValue);
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

			ObjectVertices[i] = {
				glm::vec4((float)x,objectValue[x][z],(float)z,1.0),
				//vec4(0.0,0.0,0.0,1.0),
				glm::vec3(0.0,1.0,0.0),
				glm::vec2((1.0f / (float(MAP_SIZE) - 1)) * x,(1.0f / (float(MAP_SIZE) - 1)) * z)
			};

			//std::cout << (1.0f / (float(MAP_SIZE)-1)) * (x) << ", " << (1.0f / (float(MAP_SIZE)-1)) * (z) << std::endl;
			i++;
		}
	}

	CreateShader("Shaders/CloudVertexShader.glsl", "Shaders/CloudFragmentShader.glsl");
	setupVAOVBO();
	SetImage();
}

void Cloud::Close()
{
}

void Cloud::Update()
{
}

void Cloud::Draw()
{
	unsigned int TexLoc;
	glUseProgram(programID);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	TexLoc = glGetUniformLocation(programID, "cloudTex");
	glUniform1i(TexLoc, 0);

	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, IndexData[i]);
	}
	glUseProgram(0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Cloud::SetImage() {
	glUseProgram(programID);
	unsigned int TexLoc;

	int i = 0;
	for (int x = 0; x < MAP_SIZE; x++) {
		for (int z = 0; z < MAP_SIZE; z++) {
			data[i] = 1.0f;
			data[i+1] = 1.0f;
			data[i+2] = 1.0f;
			data[i+3] = objectValue[x][z];

			i += 4;
		}
	}

	glGenTextures(1, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MAP_SIZE, MAP_SIZE, 0, GL_RGBA, GL_FLOAT, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "cloudTex");
	glUniform1i(TexLoc, 0);

}
