// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyPartEntity.h"
#include "EnemyEntity.h"
#include "EntityManager.h"
#include "BulletManager.h"

EnemyPartEntity::EnemyPartEntity(const Vec2& position, const string& texture, int health, shared_ptr<EnemyEntity> parent) :
	DamageableEntity(position, health),
	mParent(parent)
{
	mTexture.loadFromFile(texture);
	Vec2 textureSize((float)mTexture.getSize().x, (float)mTexture.getSize().y);
	mSprite.setTexture(mTexture);
	mSprite.setOrigin(textureSize * 0.5f);
}

EnemyPartEntity::~EnemyPartEntity()
{
}

void EnemyPartEntity::damage(const Vec2& direction, uint damageTaken)
{
	DamageableEntity::damage(direction, damageTaken);
}

void EnemyPartEntity::update(float dt)
{
	shared_ptr<EnemyEntity> parent = mParent.lock();
	assert(parent != shared_ptr<EnemyEntity>());

	// Aim at the player
	Vec2 worldPosition = mPosition + parent->getPosition();
	Vec2 playerDirection = worldPosition - EntityManager::inst().getPlayer()->getPosition();
	float rotation = atan2(playerDirection.y, playerDirection.x) * RAD_TO_DEG + 90.0f;
	mSprite.setRotation(rotation);

	// Fire bullets
	// TODO: Magic Numbers!!!
	if (worldPosition.y >= 0.0f && worldPosition.y < 600.0f)
	{
		if (mBulletCycle.getElapsedTime().asSeconds() > 1.0f / Game::getTimeRate())
		{
			Vec2 velocity = playerDirection; velocity *= -200.0f / sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
			BulletManager::inst().spawn(worldPosition, velocity, parent, BT_PLAYER_COMMON, false);
			mBulletCycle.restart();
		}
	}
}

void EnemyPartEntity::render(sf::RenderWindow& window)
{
	shared_ptr<EnemyEntity> parent = mParent.lock();
	assert(parent != shared_ptr<EnemyEntity>());

	// Calculate world position
	Vec2 worldPosition = mPosition + parent->getPosition();
	mSprite.setPosition(worldPosition);
	window.draw(mSprite);
}

void EnemyPartEntity::onCollision(shared_ptr<Entity> other)
{
}

sf::Sprite& EnemyPartEntity::getSprite()
{
	return mSprite;
}
