// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyPartEntity.h"
#include "EnemyEntity.h"
#include "EntityManager.h"
#include "BulletManager.h"

EnemyPartEntity::EnemyPartEntity(EnemyPartDesc& desc, shared_ptr<EnemyEntity> parent) :
	DamageableEntity(desc.position, desc.health),
	mDesc(desc),
	mParent(parent)
{
	mTexture.loadFromFile("media/" + desc.sprite);
	Vec2 textureSize((float)mTexture.getSize().x, (float)mTexture.getSize().y);
	mSprite.setTexture(mTexture);
	mSprite.setOrigin(textureSize * 0.5f);
	mSprite.setRotation(desc.rotation);
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

	Vec2 worldPosition = mPosition + parent->getPosition();
	Vec2 playerDirection = worldPosition - EntityManager::inst().getPlayer()->getPosition();

	// Aim at the player
	if (!mDesc.fixed)
	{
		float rotation = atan2(playerDirection.y, playerDirection.x) * RAD_TO_DEG - 90.0f;
		mSprite.setRotation(rotation + mDesc.rotation);
	}

	// Fire bullets
	Vec2 velocity(sin(mSprite.getRotation() * DEG_TO_RAD), -cos(mSprite.getRotation() * DEG_TO_RAD));
	velocity *= mDesc.speed;
	if (worldPosition.y >= 0.0f && worldPosition.y < Game::SCREEN_HEIGHT)
	{
		if (mBulletCycle.getElapsedTime().asSeconds() > 1.0f / Game::getTimeRate())
		{
			BulletManager::inst().spawn(worldPosition, velocity, parent, BT_ENEMY_COMMON, false);
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
