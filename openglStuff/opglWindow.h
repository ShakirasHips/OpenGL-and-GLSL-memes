#pragma once
#include <iostream>
#include <SDL.h>
#include <GL\glew.h>

class opglWindow
{
private:
	int width = 800;
	int height = 600;
	SDL_Window* window;
	SDL_GLContext openglContext;
public:
	opglWindow();
	~opglWindow();

	void init();

	SDL_Window* getWindow();

	int getWidth();
	int getHeight();
	void updateWindowParams();
};

