// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "PlayerEntity.h"
#include "BulletManager.h"

PlayerEntity::PlayerEntity(const Vec2& position, uint shipID) :
	ShipEntity(position, 1000),
	mShipID(shipID)
{
	// Load textures
	if (!mShipTexture.loadFromFile("media/player1.png"))
		throw std::exception("Failed to load player texture");
	if (!mShieldTexture.loadFromFile("media/shield.png"))
		throw std::exception("Failed to load shield texture");

	// Load gun points
	mGunPoints.push_back(Vec2(-10.0f, -16.0f));
	mGunPoints.push_back(Vec2(10.0f, -16.0f));
	mGunPoints.push_back(Vec2(-22.0f, -8.0f));
	mGunPoints.push_back(Vec2(22.0f, -8.0f));

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
		shield.rotation = 0.0f;
		mShieldSprites.push_back(shield);
	}
}

PlayerEntity::~PlayerEntity()
{
}

float step(float x, float t, float s)
{
	if (t > x)
	{
		x += s;
		return t > x ? x : t;
	}
	if (t < x)
	{
		x -= s;
		return t < x ? x : t;
	}
	return x;	
}

void PlayerEntity::damage(const Vec2& direction, uint damageTaken)
{
	ShipEntity::damage(direction, damageTaken);

	// Flare up shield
	_hitShield(direction);
}

void PlayerEntity::update(float dt)
{
	// Handle controls
	static const float MAX_VELOCITY = 125.0f;
	static const float ACCELERATION = 1000.0f;
	Vec2 targetVelocity(0.0f, 0.0f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		targetVelocity.y = -MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		targetVelocity.y = MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		targetVelocity.x = -MAX_VELOCITY;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		targetVelocity.x = MAX_VELOCITY;

	// Shooting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (mGunTimer.getElapsedTime().asSeconds() > 0.1f)
		{
			for (auto i = mGunPoints.begin(); i != mGunPoints.end(); ++i)
				BulletManager::inst().spawn(mPosition + (*i), Vec2(0.0f, -1000.0f), shared_from_this(), BT_PLAYER_COMMON);
			mGunTimer.restart();
		}
	}

	// Update velocity
	mVelocity.x = step(mVelocity.x, targetVelocity.x, ACCELERATION * dt);
	mVelocity.y = step(mVelocity.y, targetVelocity.y, ACCELERATION * dt);
	mPosition += mVelocity * dt;

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

void PlayerEntity::onCollision(shared_ptr<Entity> other)
{
	shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
	if (bullet != shared_ptr<BulletEntity>())
	{
		// Take damage
		damage(other->getPosition() - mPosition, 100);

		// Despawn the bullet
		bullet->despawn();
	}
}

sf::Sprite& PlayerEntity::getSprite()
{
	return mShipSprite;
}

void PlayerEntity::_hitShield(const Vec2& direction)
{
	// Take the front shield
	list<Shield>::iterator front = mShieldSprites.begin();

	// Flare that shield up
	(*front).visibility = 1.0f;
	(*front).rotation = atan2(direction.y, direction.x) * RAD_TO_DEG + 90.0f;

	// Move that shield to the back and shuffle the remaining shield elements forward by 1
	list<Shield>::iterator newFront = front;
	advance(newFront, 1);
	rotate(front, newFront, mShieldSprites.end());
}