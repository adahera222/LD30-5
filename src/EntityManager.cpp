// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

PlayerEntity* EntityManager::createPlayer(const Vec2& position, uint shipID)
{
	PlayerEntity* newPlayer = new PlayerEntity(position, shipID);
	mEntities.push_back(newPlayer);
	return newPlayer;
}

BulletEntity* EntityManager::createBullet()
{
	BulletEntity* newBullet = new BulletEntity;
	mBulletEntities.push_back(newBullet);
	return newBullet;
}

void EntityManager::destroyEntity(Entity* ent)
{
	if (dynamic_cast<BulletEntity*>(ent))
	{
		assert(find(mBulletEntities.begin(), mBulletEntities.end(), ent) != mBulletEntities.end());
		mBulletEntities.erase(remove(mBulletEntities.begin(), mBulletEntities.end(), ent), mBulletEntities.end());
	}
	else
	{
		assert(find(mEntities.begin(), mEntities.end(), ent) != mEntities.end());
		mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
	}
	delete ent;
}

void EntityManager::updateAll(float dt)
{
	// Update each entity
	for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
		(*i)->update(dt);
	for (auto i = mBulletEntities.begin(); i != mBulletEntities.end(); ++i)
		(*i)->update(dt);

	// Detect collisions
	for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
	{
		// Entity on Entity
		for (auto j = mEntities.begin(); j != mEntities.end(); ++j)
		{
			if (*i != *j)
			{
				bool isIntersecting = false;
				if (dynamic_cast<PlayerEntity*>(*i) || dynamic_cast<PlayerEntity*>(*j))
					isIntersecting = Collision::CircleTest((*i)->getSprite(), (*j)->getSprite());
				else
					isIntersecting = Collision::BoundingBoxTest((*i)->getSprite(), (*j)->getSprite());
				if (isIntersecting)
				{
					(*i)->onCollision(*j);
					(*j)->onCollision(*i);
				}
			}
		}

		// Bullet on Entity
		for (auto j = mBulletEntities.begin(); j != mBulletEntities.end(); ++j)
		{
			if ((*j)->isActive() && *i != (*j)->getParent())
			{
				bool isIntersecting = false;
				if (dynamic_cast<PlayerEntity*>(*i))
					isIntersecting = Collision::CircleTest((*i)->getSprite(), (*j)->getSprite());
				else
					isIntersecting = Collision::BoundingBoxTest((*i)->getSprite(), (*j)->getSprite());
				if (isIntersecting)
				{
					(*i)->onCollision(*j);
					(*j)->onCollision(*i);
				}
			}
		}
	}
}

vector<Entity*>::iterator EntityManager::getEntitiesBegin()
{
	return mEntities.begin();
}

vector<Entity*>::iterator EntityManager::getEntitiesEnd()
{
	return mEntities.end();
}

vector<BulletEntity*>::iterator EntityManager::getBulletEntitiesBegin()
{
	return mBulletEntities.begin();
}

vector<BulletEntity*>::iterator EntityManager::getBulletEntitiesEnd()
{
	return mBulletEntities.end();
}
