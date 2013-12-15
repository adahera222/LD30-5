// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Renderer.h"
#include "EntityManager.h"

Explosion::Explosion(const Vec2& position, float size) :
	mProgress(0.0f)
{
	mTexture.loadFromFile("media/explosion.png");

	Vec2 texSize((float)mTexture.getSize().x, (float)mTexture.getSize().y);
	mSprite.setTexture(mTexture);
	mSprite.setPosition(position);
	mSprite.setScale(Vec2(size / texSize.x, size / texSize.y));
}

Explosion::~Explosion()
{
}

bool Explosion::update(float dt)
{
	mProgress = step(mProgress, 1.0f, dt);

	if (mProgress == 1.0f)
		return false;

	// Update texture here

	return true;
}

void Explosion::render(sf::RenderWindow& window)
{
	window.draw(mSprite);
}

Renderer::Renderer(uint width, uint height, bool fullscreen) :
	mWindow(sf::VideoMode(width, height), "LD28", fullscreen ? sf::Style::Fullscreen : 7U)
{
	// Set window settings
	mWindow.setVerticalSyncEnabled(true);
}

Renderer::~Renderer()
{
}

void Renderer::createExplosion(const Vec2& position, float size)
{
	shared_ptr<Explosion> exp(new Explosion(position, size));
	mExplosions.push_back(exp);
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

void Renderer::render(float dt)
{
	mWindow.clear(sf::Color::Black);

	// Draw entities
	for (auto i = EntityManager::inst().getEntitiesBegin(); i != EntityManager::inst().getEntitiesEnd(); ++i)
		(*i)->render(mWindow);
	for (auto i = EntityManager::inst().getPlayerBulletsBegin(); i != EntityManager::inst().getPlayerBulletsEnd(); ++i)
		(*i)->render(mWindow);
	for (auto i = EntityManager::inst().getNonPlayerBulletsBegin(); i != EntityManager::inst().getNonPlayerBulletsEnd(); ++i)
		(*i)->render(mWindow);
	for (auto i = EntityManager::inst().getSpecialWeaponsBegin(); i != EntityManager::inst().getSpecialWeaponsEnd(); ++i)
		(*i)->render(mWindow);
	EntityManager::inst().getPlayer()->_drawLocks(mWindow);

	// Update effects
	for (auto i = mExplosions.begin(); i != mExplosions.end(); ++i)
	{
		(*i)->update(dt);
		(*i)->render(mWindow);
	}

	mWindow.display();
}

sf::RenderWindow& Renderer::getWindow()
{
	return mWindow;
}
