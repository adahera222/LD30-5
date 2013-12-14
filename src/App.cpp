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

	// Create a test scene
	PlayerEntity* player = EntityManager::inst().createPlayer(Vec2(400, 300), 1);

	// Run the program as long as the window is open
	sf::Clock clock;
	while (Renderer::inst().getWindow().isOpen())
	{
		// Get time elapsed since last frame
		float time = clock.getElapsedTime().asSeconds();
		if (time < EPSILON)
			time = 1.0f / 60.0f;
		clock.restart();

		// Run a frame
		Renderer::inst().processEvents();
		EntityManager::inst().updateAll(time);
		Renderer::inst().render();
	}

	return EXIT_SUCCESS;
}
