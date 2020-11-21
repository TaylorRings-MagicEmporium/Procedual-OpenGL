#include "Terrain.h"

void Terrain::Setup() {

	objectValue = new float* [MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; i++) objectValue[i] = new float[MAP_SIZE];

	for (int x = 0; x < MAP_SIZE; x++)
	{
		for (int z = 0; z < MAP_SIZE; z++)
		{
			objectValue[x][z] = 0;
		}
	}

	srand(2);

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

	//computing normal vectors
	int index1, index2, index3;
	glm::vec3 p1, p2, p3;
	glm::vec3 edge1, edge2;
	glm::vec3 crossNorm, finalNorm;
	for (int z = 0; z < MAP_SIZE - 1; z++) {
		for (int x = 0; x < (MAP_SIZE * 2 - 2); x++) {
			index1 = IndexData[z][x];
			index2 = IndexData[z][x + 1];
			index3 = IndexData[z][x + 2];

			p1 = glm::vec3(ObjectVertices[index1].coords);
			p2 = glm::vec3(ObjectVertices[index2].coords);
			p3 = glm::vec3(ObjectVertices[index3].coords);

			edge1 = p2 - p1;
			edge2 = p3 - p1;

			//crossNorm = cross(edge2, edge1);
			if (x % 2 == 1) {
				crossNorm = cross(edge2, edge1);
			}
			else {
				crossNorm = cross(edge1, edge2);
			}
			if (glm::dot(crossNorm, glm::vec3(0, 1, 0)) < 1e-06f) {
				finalNorm = -crossNorm;
			}
			else {
				finalNorm = crossNorm;
			}

			//finalNorm = crossNorm;

			ObjectVertices[index1].normal = finalNorm + ObjectVertices[index1].normal;
			ObjectVertices[index2].normal = finalNorm + ObjectVertices[index2].normal;
			ObjectVertices[index3].normal = finalNorm + ObjectVertices[index3].normal;

		}
	}

	//normalising normals
	for (int i = 0; i < (MAP_SIZE * MAP_SIZE) - 1; i++) {
		ObjectVertices->normal = glm::normalize(ObjectVertices->normal);
	}

	// Create shader program executable - read, compile and link shaders
	CreateShader("Shaders/TerrainVertexShader.glsl", "Shaders/TerrainFragmentShader.glsl");
	setupVAOVBO();
	SetImages();


}

void Terrain::Close() {
	delete[] objectValue;
}

void Terrain::Update() {

}

void Terrain::Draw() {
	glUseProgram(programID);
	glBindVertexArray(vao);

	unsigned int TexLoc;

	glActiveTexture(GL_TEXTURE0 + SAND);
	glBindTexture(GL_TEXTURE_2D, texture[SAND]);
	TexLoc = glGetUniformLocation(programID, "sandTex");
	glUniform1i(TexLoc, SAND);
	glActiveTexture(GL_TEXTURE0 + GRASS);
	glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
	TexLoc = glGetUniformLocation(programID, "grassTex");
	glUniform1i(TexLoc, GRASS);
	glActiveTexture(GL_TEXTURE0 + ROCK);
	glBindTexture(GL_TEXTURE_2D, texture[ROCK]);
	TexLoc = glGetUniformLocation(programID, "rockTex");
	glUniform1i(TexLoc, ROCK);
	glActiveTexture(GL_TEXTURE0 + SNOW);
	glBindTexture(GL_TEXTURE_2D, texture[SNOW]);
	TexLoc = glGetUniformLocation(programID, "snowTex");
	glUniform1i(TexLoc, SNOW);

	for (int i = 0; i < MAP_SIZE - 1; i++)
	{
		glDrawElements(GL_TRIANGLE_STRIP, verticesPerStrip, GL_UNSIGNED_INT, IndexData[i]);
	}
	glUseProgram(0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::SetImages() {
	glUseProgram(programID);
	unsigned int TexLoc;
	image[GRASS] = getbmp("Textures/grass.bmp");

	glGenTextures(4, texture);
	glActiveTexture(GL_TEXTURE0 + GRASS);
	glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[GRASS]->sizeX, image[GRASS]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[GRASS]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "grassTex");
	glUniform1i(TexLoc, GRASS);

	//glBindTexture(GL_TEXTURE_2D, 0);
	//////////////////////////////////////

	image[SAND] = getbmp("Textures/sand.bmp");

	glActiveTexture(GL_TEXTURE0 + SAND);
	glBindTexture(GL_TEXTURE_2D, texture[SAND]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[SAND]->sizeX, image[SAND]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[SAND]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "sandTex");
	glUniform1i(TexLoc, SAND);

	////////////////////////////////////
	image[ROCK] = getbmp("Textures/rock.bmp");

	glActiveTexture(GL_TEXTURE0 + ROCK);
	glBindTexture(GL_TEXTURE_2D, texture[ROCK]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[ROCK]->sizeX, image[ROCK]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[ROCK]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "rockTex");
	glUniform1i(TexLoc, ROCK);

	////////////////////////////////////
	image[SNOW] = getbmp("Textures/snow.bmp");

	glActiveTexture(GL_TEXTURE0 + SNOW);
	glBindTexture(GL_TEXTURE_2D, texture[SNOW]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[SNOW]->sizeX, image[SNOW]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[SNOW]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//?

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//?

	glGenerateMipmap(GL_TEXTURE_2D);

	TexLoc = glGetUniformLocation(programID, "snowTex");
	glUniform1i(TexLoc, SNOW);

}
