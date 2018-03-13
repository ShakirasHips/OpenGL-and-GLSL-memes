#include "opglWindow.h"
#include <string>
#include <fstream>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "ShapeData.h"
#include "ShapeGen.h"
#include "Camera.h"

#define log(x) std::cout << x << std::endl

Camera cam;
int numIndicies;

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

void mouseMoveEvent(SDL_MouseMotionEvent* e)
{
	cam.mouseUpdate(glm::vec2(e->x, e->y));
}

void keyBoardStateChecker(const Uint8 *keyboardState)
{
	if (keyboardState[SDL_SCANCODE_W])
	{
		cam.moveForward();
	}
	else if (keyboardState[SDL_SCANCODE_S])
	{
		cam.moveBackward();
	}
	
	if (keyboardState[SDL_SCANCODE_A])
	{
		cam.moveLeft();
	}
	else if (keyboardState[SDL_SCANCODE_D])
	{
		cam.moveRight();
	}

	if (keyboardState[SDL_SCANCODE_F])
	{
		cam.moveDown();
	}
	else if (keyboardState[SDL_SCANCODE_R])
	{
		cam.moveUp();
	}

}

void sendDataToGL()
{
	ShapeData shape = ShapeGen::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIndicies = shape.numIndicies;
	shape.Cleanup();


	GLuint transformationMatrixBufferID;
	glGenBuffers(1, &transformationMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);


	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*2, 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FLOAT, sizeof(glm::mat4), (void*)(sizeof(float) * 0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FLOAT, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FLOAT, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FLOAT, sizeof(glm::mat4), (void*)(sizeof(float) * 12));


	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);


	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);


}

void loadShaders()
{
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
		return exit(2);


	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	if (!checkProgramLinkStatus(programID))
		return exit(2);

	glUseProgram(programID);
}

int main(int argc, char** argv)
{
	opglWindow window;
	window.init();
	SDL_Event e;

	loadShaders();
	sendDataToGL();

	for (;;)
	{
		SDL_PollEvent(&e);
		
		if (e.type == SDL_QUIT)
		{
			break;
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			mouseMoveEvent(&e.motion);
		}
		
		keyBoardStateChecker(SDL_GetKeyboardState(0));

		glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), (float)window.getWidth()/ window.getHeight(), 0.1f, 100.0f);
		glm::mat4 fullTransforms[] =
		{
			glm::rotate(glm::translate(projectionMatrix * cam.getWorldToViewMatrix(), glm::vec3(0.0f, 0.0f, -3.0f)), glm::radians(54.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
			glm::rotate(glm::translate(projectionMatrix * cam.getWorldToViewMatrix(), glm::vec3(2.0f, 1.0f, -3.5f)), glm::radians(126.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_DYNAMIC_DRAW);

		glViewport(0, 0, window.getWidth(), window.getHeight());
		window.updateWindowParams();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		
		glDrawElementsInstanced(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, 0 , 2);
		glDrawElementsInstanced(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, 0, 1);

		SDL_GL_SwapWindow(window.getWindow());
	}
	
	return 0;
}