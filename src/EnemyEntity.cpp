// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyEntity.h"
#include "EntityManager.h"

EnemyEntity::EnemyEntity(const Vec2& position, float speed) :
	ShipEntity(position, 100),
	mSpeed(speed)
{
	mTexture.loadFromFile("media/enemy1.png");
	mSprite.setTexture(mTexture);
}

EnemyEntity::~EnemyEntity()
{
}

void EnemyEntity::damage(const Vec2& direction, uint damageTaken)
{
	ShipEntity::damage(direction, damageTaken);
}

void EnemyEntity::update(float dt)
{
	mPosition += Vec2(0.0f, mSpeed * dt);
}

void EnemyEntity::render(sf::RenderWindow& window)
{
	mSprite.setPosition(mPosition);
	window.draw(mSprite);
}

void EnemyEntity::onCollision(shared_ptr<Entity> other)
{
	shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
	if (bullet != shared_ptr<BulletEntity>())
	{
		// Take damage
		damage(other->getPosition() - mPosition, 50);

		// Despawn bullet
		bullet->despawn();
	}

	// If we're dead - remove ourselves
	if (mHealth < 0)
		EntityManager::inst().destroyEntity(shared_from_this());
}

sf::Sprite& EnemyEntity::getSprite()
{
	return mSprite;
}
