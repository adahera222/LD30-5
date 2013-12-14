/*
	Ludum Dare 28 Entry
	Copyright (c) David Avedissian 2013
*/
#include "Common.h"
#include "App.h"
#include "Renderer.h"

App::App()
{
}

App::~App()
{
}

int App::run()
{
	new Renderer(800, 600, false);

	// Run the program as long as the window is open
	while (Renderer::inst().getWindow().isOpen())
	{
		Renderer::inst().render();
		Renderer::inst().processEvents();
	}

	return EXIT_SUCCESS;
}
