// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Game.h"
#include "Renderer.h"
#include "EntityManager.h"
#include "BulletManager.h"

int Game::run()
{
	new Renderer(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, false);
	new EntityManager;
	new BulletManager;

	// SURVIVAL MODE:
	// Score slowly increases, number of players and rate of spawn is proportional to score
	EntityManager::inst().createPlayer(Vec2((float)Game::SCREEN_WIDTH / 2, (float)Game::SCREEN_HEIGHT * 2.0f / 3.0f), 1);

	sf::Clock scoreTimer;
	sf::Clock spawnTimer;
	sf::Clock bossWaitTimer;
	bool bossSpawned = false;
	int score = 0;
	int rowSize = 2;
	float spawnTime = 5.0f;

	// Run the program as long as the window is open
	sf::Clock clock;
	while (Renderer::inst().getWindow().isOpen())
	{
		// Get time elapsed since last frame
		float time = clock.getElapsedTime().asSeconds();
		if (time < EPSILON)
			time = 1.0f / 60.0f;
		clock.restart();

		// SURVIVAL MODE:
		if (scoreTimer.getElapsedTime().asSeconds() > 1.0f / getTimeRate())
		{
			score += 100;
			scoreTimer.restart();

			// Increase difficulty
			if (score < 10000)
				rowSize = 2 + score / 1000;

			// Spawn the big boss
			if (score == 6000)
			{
				bossWaitTimer.restart();
				bossSpawned = true;
				EntityManager::inst().createEnemy(Vec2(Game::SCREEN_WIDTH / 2 - 64.0f, -64.0f), "boss1");
			}
		}
		if (spawnTimer.getElapsedTime().asSeconds() > 4.0f / getTimeRate() &&
			(!bossSpawned || bossWaitTimer.getElapsedTime().asSeconds() > 10.0f / getTimeRate()))
		{
			string enemy = rowSize % 2 == 0 ? "enemy2" : "enemy1";
			for (int x = 1; x < (rowSize + 1); x++)
				EntityManager::inst().createEnemy(Vec2((float)x * Game::SCREEN_WIDTH / (rowSize + 1) - 16.0f, -16.0f), enemy);
			spawnTimer.restart();
		}

		// Run a frame
		Renderer::inst().processEvents();
		EntityManager::inst().updateAll(time * getTimeRate());
		Renderer::inst().render();
	}

	// Tear down
	BulletManager::release();
	EntityManager::release();
	Renderer::release();

	return EXIT_SUCCESS;
}

float Game::getTimeRate()
{
	return EntityManager::inst().getPlayer()->inBulletTime() ? 0.25f : 1.0f;
}
