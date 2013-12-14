// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(const Vec2& position, uint shipID) :
	Entity(position),
	mShipID(shipID)
{
	// Load textures
	if (!mShipTexture.loadFromFile("media/player1.png"))
		throw std::exception("Failed to load player texture");
	if (!mShieldTexture.loadFromFile("media/shield.png"))
		throw std::exception("Failed to load shield texture");

	// Set up ship sprite
	Vec2 shipTextureSize((float)mShipTexture.getSize().x, (float)mShipTexture.getSize().y);
	mShipSprite.setTexture(mShipTexture);
	mShipSprite.setOrigin(shipTextureSize * 0.5f);

	// Set up shield sprites
	Vec2 shieldTextureSize((float)mShieldTexture.getSize().x, (float)mShieldTexture.getSize().y);
	for (uint i = 0; i < 5; ++i)
	{
		Shield shield;
		shield.sprite.setTexture(mShieldTexture);
		shield.sprite.setOrigin(shieldTextureSize * 0.5f);
		shield.visibility = 0.0f;
		shield.rotation = random(0.0f, 360.0f);
		mShieldSprites.push_back(shield);
	}
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::update(float dt)
{
	static sf::Clock clock;
	if (clock.getElapsedTime() > sf::seconds(0.2f))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		{
			_hitShield(Vec2(random(-1.0f, 1.0f), random(-1.0f, 1.0f)));
			clock.restart();
		}
	}

	// Fade out shields
	for (auto i = mShieldSprites.begin(); i != mShieldSprites.end(); ++i)
	{
		if ((*i).visibility > EPSILON)
		{
			(*i).visibility -= dt;
			if ((*i).visibility < 0.0f)
				(*i).visibility = 0.0f;
		}
	}
}

void PlayerEntity::render(sf::RenderWindow& window)
{
	mShipSprite.setPosition(mPosition);
	window.draw(mShipSprite);

	// Draw shields
	for (auto i = mShieldSprites.begin(); i != mShieldSprites.end(); ++i)
	{
		if ((*i).visibility > EPSILON)
		{
			(*i).sprite.setPosition(mPosition);
			(*i).sprite.setRotation((*i).rotation);
			(*i).sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (*i).visibility)));
			window.draw((*i).sprite);
		}
	}
}

void PlayerEntity::_hitShield(const Vec2& direction)
{
	// Take the front shield
	list<Shield>::iterator front = mShieldSprites.begin();

	// Flare that shield up
	(*front).visibility = 1.0f;
	(*front).rotation = atan2(direction.y, direction.x) * RAD_TO_DEG;

	// Move that shield to the back and shuffle the remaining shield elements forward by 1
	list<Shield>::iterator newFront = front;
	advance(newFront, 1);
	rotate(front, newFront, mShieldSprites.end());
}
