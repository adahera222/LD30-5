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
	for (uint i = 0; i < 50; ++i)
		mPlayerBulletCache.push_back(EntityManager::inst().createBullet(true));
	for (uint i = 0; i < 500; ++i)
		mNonPlayerBulletCache.push_back(EntityManager::inst().createBullet(false));
}

BulletManager::~BulletManager()
{
	for (auto i = mNonPlayerBulletCache.begin(); i != mNonPlayerBulletCache.end(); ++i)
		EntityManager::inst().destroyEntity(*i);
	mNonPlayerBulletCache.clear();

	for (auto i = mPlayerBulletCache.begin(); i != mPlayerBulletCache.end(); ++i)
		EntityManager::inst().destroyEntity(*i);
	mPlayerBulletCache.clear();
}

void BulletManager::spawn(const Vec2& position, const Vec2& velocity, shared_ptr<Entity> parent, BulletType type, bool friendly)
{
	auto& cache = friendly ? mPlayerBulletCache : mNonPlayerBulletCache;

	// Take first bullet
	auto front = cache.begin();

	// Spawn it
	(*front)->spawn(position, velocity, parent, mBulletTextures[type]);

	// Move this bullet to the back of the cache
	auto newFront = front;
	advance(newFront, 1);
	rotate(front, newFront, cache.end());
}
