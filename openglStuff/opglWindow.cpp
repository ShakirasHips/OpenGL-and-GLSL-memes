#include "opglWindow.h"



opglWindow::opglWindow()
{
}


opglWindow::~opglWindow()
{
	SDL_GL_DeleteContext(openglContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void opglWindow::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
	window = SDL_CreateWindow("Memes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);
	openglContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew failed to init" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);

}

SDL_Window * opglWindow::getWindow()
{
	return window;
}

int opglWindow::getWidth()
{
	return width;
}

int opglWindow::getHeight()
{
	return height;
}

void opglWindow::updateWindowParams()
{
	SDL_GetWindowSize(window, &width, &height);
}
