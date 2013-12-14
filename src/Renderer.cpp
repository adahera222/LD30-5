// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Renderer.h"
#include "EntityManager.h"

Renderer::Renderer(uint width, uint height, bool fullscreen) :
	mWindow(sf::VideoMode(width, height), "LD28")
{
	// Set window settings
	mWindow.setVerticalSyncEnabled(true);
}

Renderer::~Renderer()
{
}

void Renderer::processEvents()
{
	// Check all the window's events that were triggered since the last iteration of the loop
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				mWindow.close();
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;

		default:
			break;
		}
	}
}

void Renderer::render()
{
	mWindow.clear(sf::Color::Black);

	// Draw entities
	for (auto i = EntityManager::inst().getEntitiesBegin(); i != EntityManager::inst().getEntitiesEnd(); ++i)
		(*i)->render(mWindow);
	for (auto i = EntityManager::inst().getBulletEntitiesBegin(); i != EntityManager::inst().getBulletEntitiesEnd(); ++i)
		(*i)->render(mWindow);

	mWindow.display();
}

sf::RenderWindow& Renderer::getWindow()
{
	return mWindow;
}
