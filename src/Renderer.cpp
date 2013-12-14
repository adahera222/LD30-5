/*
	Ludum Dare 28 Entry
	Copyright (c) David Avedissian 2013
*/
#include "Common.h"
#include "Renderer.h"

Renderer::Renderer(uint width, uint height, bool fullscreen) :
	mWindow(sf::VideoMode(width, height), "LD28")
{
	// Set window settings
	mWindow.setVerticalSyncEnabled(true);
}

Renderer::~Renderer()
{
}

void Renderer::render()
{
}

void Renderer::processEvents()
{
	// Check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

sf::Window& Renderer::getWindow()
{
	return mWindow;
}
