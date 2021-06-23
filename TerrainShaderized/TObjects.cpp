#include "TObjects.h"

// checks that the shader is compiled correctly with out syntax errors 
void TObjects::shaderCompileTest(GLuint shader) {
	GLint result = GL_FALSE;
	int logLength;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}
// reads a text file and outputs the data of the file
char* TObjects::readTextFile(char* aTextFile)
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

// createShader requires a vertex shader and a fragment shader and contructs a shader program id for the component.
void TObjects::CreateShader(std::string vertexShad, std::string fragShad) {
	unsigned int vertexShaderId, fragmentShaderId;
	char* vertexShader = readTextFile((char*)vertexShad.c_str());
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, (const char**)&vertexShader, NULL);
	glCompileShader(vertexShaderId);
	std::cout << "VERTEX::" << std::endl;
	shaderCompileTest(vertexShaderId);

	char* fragmentShader = readTextFile((char*)fragShad.c_str());
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char**)&fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	std::cout << "FRAGMENT::" << std::endl;
	shaderCompileTest(fragmentShaderId);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderId);
	glAttachShader(programID, fragmentShaderId);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

// sets the program's shaders with the updated projection map and model view matrix
void TObjects::SetCamera(glm::mat4 projMat, glm::mat4 modelView) {
	glUseProgram(programID);
	unsigned int projMatLoc = glGetUniformLocation(programID, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
	
	unsigned int modelViewMatLoc = glGetUniformLocation(programID, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelView));
	this->modelView = modelView;
	glUseProgram(0);
}

// creates a normal map for the modelview matrix. this should be run once as it can cause shadow issues when moving.
void TObjects::SetNormalMat() {
	glUseProgram(programID);
	unsigned int normalMatLoc = glGetUniformLocation(programID, "normalMat");
	glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelView)));
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, value_ptr(normalMat));
	glUseProgram(0);

}
// set's the program with the material and lighting
void TObjects::SetMandL(Material m, Light l) {
	glUseProgram(programID);

	glUniform4fv(glGetUniformLocation(programID, "terrainFandB.ambRefl"), 1, &m.ambRefl[0]);
	glUniform4fv(glGetUniformLocation(programID, "terrainFandB.difRefl"), 1, &m.difRefl[0]);
	glUniform4fv(glGetUniformLocation(programID, "terrainFandB.specRefl"), 1, &m.specRefl[0]);
	glUniform4fv(glGetUniformLocation(programID, "terrainFandB.emitCols"), 1, &m.emitCols[0]);
	glUniform1f(glGetUniformLocation(programID, "terrainFandB.shininess"), m.shininess);

	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programID, "globAmb"), 1, &globAmb[0]);

	///////////////////////////////////////

	glUniform4fv(glGetUniformLocation(programID, "light0.ambCols"), 1, &l.ambCols[0]);
	glUniform4fv(glGetUniformLocation(programID, "light0.difCols"), 1, &l.difCols[0]);
	glUniform4fv(glGetUniformLocation(programID, "light0.specCols"), 1, &l.specCols[0]);
	glUniform4fv(glGetUniformLocation(programID, "light0.coords"), 1, &l.coords[0]);

	glUseProgram(0);
}

// updates the program with a defined value for a name
void TObjects::SetFloat(float da, std::string name) {
	glUseProgram(programID);
	unsigned int Loc = glGetUniformLocation(programID, name.c_str());
	glUniform1f(Loc, da);
}

// updates the program with a defined value for a name
void TObjects::SetVec3(glm::vec3 da, std::string name) {
	glUseProgram(programID);
	unsigned int Loc = glGetUniformLocation(programID, name.c_str());
	glUniform3fv(Loc, 1, value_ptr(da));
}

// updates the program with a defined value for a name
void TObjects::SetVec4(glm::vec4 da, std::string name) {
	glUseProgram(programID);
	unsigned int Loc = glGetUniformLocation(programID, name.c_str());
	glUniform4fv(Loc, 1, value_ptr(da));
}