// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "App.h"
#include "Renderer.h"
#include "EntityManager.h"

App::App()
{
}

App::~App()
{
}

int App::run()
{
	new Renderer(800, 600, false);
	new EntityManager;

	// Run the program as long as the window is open
	while (Renderer::inst().getWindow().isOpen())
	{
		Renderer::inst().processEvents();
		Renderer::inst().render();
	}

	return EXIT_SUCCESS;
}
