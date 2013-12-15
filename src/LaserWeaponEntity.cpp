// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "LaserWeaponEntity.h"
#include "EntityManager.h"

LaserWeaponEntity::LaserWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target) :
	SpecialWeaponEntity(startingPosition, target),
	mProgress(0.0f)
{
	mSquare.loadFromFile("media/square.jpg");
	Vec2 size((float)mSquare.getSize().x, (float)mSquare.getSize().y);
	mCollisionBox.setTexture(mSquare);
	mCollisionBox.setOrigin(size * 0.5f);
}

LaserWeaponEntity::~LaserWeaponEntity()
{
}

void LaserWeaponEntity::update(float dt)
{
	// Move towards target
	mProgress = step(mProgress, 1.0f, 1.0f * dt);
	shared_ptr<DamageableEntity> target = mTarget.lock();
	if (target != shared_ptr<DamageableEntity>())
		mPosition = mStartPosition * (1.0f - mProgress) + target->getSprite().getPosition() * mProgress;
	else
		EntityManager::inst().destroyEntity(shared_from_this());
}

void LaserWeaponEntity::render(sf::RenderWindow& window)
{
	mCollisionBox.setPosition(mPosition);
}

void LaserWeaponEntity::onCollision(shared_ptr<Entity> other)
{
}

sf::Sprite& LaserWeaponEntity::getSprite()
{
	return mCollisionBox;
}
