// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EnemyEntity.h"
#include "EntityManager.h"

EnemyEntity::EnemyEntity(const Vec2& position, EnemyDesc& desc) :
	DamageableEntity(position, desc.health),
	mDesc(desc),
	mSpeed(desc.speed)
{
	// TODO: ERROR CHECKING
	mTexture.loadFromFile("media/" + desc.sprite);
	Vec2 textureSize((float)mTexture.getSize().x, (float)mTexture.getSize().y);
	mSprite.setTexture(mTexture);
	mSprite.setOrigin(textureSize * 0.5f);
}

EnemyEntity::~EnemyEntity()
{
	mParts.clear();
}

void EnemyEntity::_setupParts()
{
	for (auto i = mDesc.parts.begin(); i != mDesc.parts.end(); ++i)
	{
		shared_ptr<EnemyPartEntity> part = EntityManager::inst().createEnemyPart(*i, shared_from_this());
		mParts.push_back(part);
	}
}

void EnemyEntity::_onDestroy()
{
	for (auto i = mParts.begin(); i != mParts.end(); ++i)
		EntityManager::inst().destroyEntity(*i);
	mParts.clear();
}

void EnemyEntity::damage(const Vec2& direction, uint damageTaken)
{
	DamageableEntity::damage(direction, damageTaken);
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
	DamageableEntity::onCollision(other);

	// If we're dead - remove ourselves
	if (mHealth < 0)
		EntityManager::inst().destroyEntity(shared_from_this());
}

sf::Sprite& EnemyEntity::getSprite()
{
	return mSprite;
}
