#include "opglWindow.h"
#include <string>
#include <fstream>
#include <glm.hpp>

#define log(x) std::cout << x << std::endl


bool checkShaderCompilerStatus(GLuint shaderID)
{
	GLint compilerStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilerStatus);
	if (compilerStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		log(buffer);

		delete[] buffer;
		return false;
	}

	return true;
}


bool checkProgramLinkStatus(GLuint shaderID)
{
	GLint linkStatus;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		glGetProgramInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
		log(buffer);

		delete[] buffer;
		return false;
	}

	return true;
}

std::string readShaderCode(const char* filename)
{
	std::ifstream input(filename);
	if (!input.good())
	{
		std::cout << filename << " " << "failed to load" << std::endl;
		exit(1);
	}

	return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());

}

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;

};

const float xdel = 0.1f;
int numTrig = 0;
const int vertsTri = 3;
const int numfloatsvert = 6;
const int trianglebytesize = numfloatsvert*vertsTri * sizeof(float);



void sendAnotherTriToOpenGL()
{
	const GLfloat tri = -1 + numTrig * xdel;

	GLfloat g[] = 
	{
		tri, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		tri+xdel, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		tri, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	glBufferSubData(GL_ARRAY_BUFFER, numTrig*trianglebytesize, trianglebytesize, g);
	numTrig++;

}

void drawTrig()
{
	Vertex tri[] =
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tri), tri);
}


int main(int argc, char** argv)
{
	opglWindow window;
	window.init();
	SDL_Event e;

	

	GLuint vertBufferID;
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, 10000, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));


	GLushort ind[] = {0,1,2, 3, 4,5};
	GLuint indBufferID;
	glGenBuffers(1, &indBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);


	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	std::string temp = readShaderCode("vertexShaderCode.glsl").c_str();
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);

	temp = readShaderCode("fragShaderCode.glsl").c_str();
	adapter[0] = temp.c_str();
	glShaderSource(fragShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragShaderID);
	
	if (!checkShaderCompilerStatus(vertexShaderID) || !checkShaderCompilerStatus(fragShaderID))
		return 1;


	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	if (!checkProgramLinkStatus(programID))
		return 2;

	glUseProgram(programID);
	
	
	drawTrig();

	for (;;)
	{
		SDL_PollEvent(&e);
		
		if (e.type == SDL_QUIT)
		{
			break;
		}
		
		glViewport(0, 0, window.getWidth(), window.getHeight());
		window.updateWindowParams();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		SDL_GL_SwapWindow(window.getWindow());
	}
	
	return 0;
}