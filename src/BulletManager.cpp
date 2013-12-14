// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "BulletManager.h"
#include "EntityManager.h"

BulletManager::BulletManager()
{
	assert(EntityManager::ptr());

	// Load bullet textures
	sf::Texture playerBullet;
	playerBullet.loadFromFile("media/bullet-player-common.png");
	mBulletTextures.insert(make_pair(BT_PLAYER_COMMON, playerBullet));

	// Create initial bullets
	for (uint i = 0; i < 100; ++i)
	{
		BulletEntity* ent = EntityManager::inst().createBullet();
		mBulletCache.push_back(ent);
	}
}

BulletManager::~BulletManager()
{
	for (auto i = mBulletCache.begin(); i != mBulletCache.end(); ++i)
		EntityManager::inst().destroyEntity(*i);
	mBulletCache.clear();
}

void BulletManager::spawn(const Vec2& position, const Vec2& velocity, Entity* parent, BulletType type)
{
	// Take first bullet
	list<BulletEntity*>::iterator front = mBulletCache.begin();

	// Spawn it
	(*front)->spawn(position, velocity, parent, mBulletTextures[type]);

	// Move this bullet to the back of the cache
	list<BulletEntity*>::iterator newFront = front;
	advance(newFront, 1);
	rotate(front, newFront, mBulletCache.end());
}
