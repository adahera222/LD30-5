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

shared_ptr<PlayerEntity> EntityManager::createPlayer(const Vec2& position, uint shipID)
{
	shared_ptr<PlayerEntity> newPlayer(new PlayerEntity(position, shipID));
	mEntities.push_back(newPlayer);
	return newPlayer;
}

shared_ptr<EnemyEntity> EntityManager::createEnemy(const Vec2& position, float speed)
{
	shared_ptr<EnemyEntity> newEnemy(new EnemyEntity(position, speed));
	mEntities.push_back(newEnemy);
	return newEnemy;
}

shared_ptr<BulletEntity> EntityManager::createBullet()
{
	shared_ptr<BulletEntity> newBullet(new BulletEntity);
	mBulletEntities.push_back(newBullet);
	return newBullet;
}

void EntityManager::destroyEntity(shared_ptr<Entity> ent)
{
	if (dynamic_pointer_cast<BulletEntity>(ent) != shared_ptr<BulletEntity>())
		mBulletEntities.erase(remove(mBulletEntities.begin(), mBulletEntities.end(), ent), mBulletEntities.end());
	else
		mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
}

void EntityManager::updateAll(float dt)
{
	// Update each entity
	for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
		(*i)->update(dt);
	for (auto i = mBulletEntities.begin(); i != mBulletEntities.end(); ++i)
		(*i)->update(dt);

	// Detect collisions
	vector<shared_ptr<Entity>> currentEntities = mEntities;
	for (auto i = currentEntities.begin(); i != currentEntities.end(); ++i)
	{
		// Entity on Entity
		for (auto j = i + 1; j != currentEntities.end(); ++j)
		{
			bool isIntersecting = false;
			if (dynamic_pointer_cast<PlayerEntity>(*i) != shared_ptr<PlayerEntity>() ||
				dynamic_pointer_cast<PlayerEntity>(*j) != shared_ptr<PlayerEntity>())
				isIntersecting = Collision::CircleTest((*i)->getSprite(), (*j)->getSprite());
			else
				isIntersecting = Collision::BoundingBoxTest((*i)->getSprite(), (*j)->getSprite());
			if (isIntersecting)
			{
				(*i)->onCollision(*j);
				(*j)->onCollision(*i);
			}
		}

		// Bullet on Entity
		for (auto j = mBulletEntities.begin(); j != mBulletEntities.end(); ++j)
		{
			if ((*j)->isActive() && *i != (*j)->getParent())
			{
				bool isIntersecting = false;
				if (dynamic_pointer_cast<PlayerEntity>(*i) != shared_ptr<PlayerEntity>())
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

vector<shared_ptr<Entity>>::iterator EntityManager::getEntitiesBegin()
{
	return mEntities.begin();
}

vector<shared_ptr<Entity>>::iterator EntityManager::getEntitiesEnd()
{
	return mEntities.end();
}

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getBulletEntitiesBegin()
{
	return mBulletEntities.begin();
}

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getBulletEntitiesEnd()
{
	return mBulletEntities.end();
}