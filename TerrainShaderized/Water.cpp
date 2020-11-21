#include "Water.h"

void Water::Setup()
{
	objectValue = new float* [MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; i++) objectValue[i] = new float[MAP_SIZE];

	int i = 0;

	for (int z = 0; z < MAP_SIZE; z++)
	{
		for (int x = 0; x < MAP_SIZE; x++)
		{
			objectValue[x][z] = 0;

			ObjectVertices[i] = {
				glm::vec4((float)x,objectValue[x][z],(float)z,1.0),
				glm::vec3(0.0,1.0,0.0),
				glm::vec2((1.0f / (float(MAP_SIZE) - 1)) * x,(1.0f / (float(MAP_SIZE) - 1)) * z)
			};

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
			IndexData[z][x] = i;
			i++;
		}
		for (int x = 1; x < MAP_SIZE * 2 + 1; x += 2)
		{
			IndexData[z][x] = i;
			i++;
		}
	}

	CreateShader("Shaders/WaterVertexShader.glsl", "Shaders/WaterFragmentShader.glsl");
	setupVAOVBO();
	SetImages();
}

void Water::Close()
{
	delete[] objectValue;
}

void Water::Update()
{	
	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<float> d = endTime - beginTime;
	float ti = d.count();
	//std::cout << ti << std::endl;

	SetFloat(ti,"waterFlowRate");
}

void Water::Draw()
{
	glUseProgram(programID);
	glBindVertexArray(vao);

	unsigned int TexLoc;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	TexLoc = glGetUniformLocation(programID, "waterTex");
	glUniform1i(TexLoc, 0);

	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, IndexData[i]);
	}
	glUseProgram(0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Water::SetImages()
{
	glUseProgram(programID);
	unsigned int TexLoc;
	//image[0] = getbmp("Textures/water.bmp");
	int width, height;
	unsigned char* data = SOIL_load_image("Textures/water.bmp", &width, &height, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, texture);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "waterTex");
	glUniform1i(TexLoc, 0);

	
}
