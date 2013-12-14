// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "App.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "BulletManager.h"

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
	new BulletManager;

	// Create a test scene
	EntityManager::inst().createPlayer(Vec2(400, 500), 1);

	for (int y = -1000; y <= 0; y += 200)
	{
		for (int x = 100; x < 800; x += 200)
		{
			EntityManager::inst().createEnemy(Vec2(x, y), 50.0f);
		}
	}

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

	// Tear down
	BulletManager::release();
	EntityManager::release();
	Renderer::release();

	return EXIT_SUCCESS;
}
