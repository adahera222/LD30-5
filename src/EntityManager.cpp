// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
	mNonPlayerBullets.clear();
	mPlayerBullets.clear();
	mEntities.clear();
}

shared_ptr<PlayerEntity> EntityManager::createPlayer(const Vec2& position, uint shipID)
{
	assert(mPlayer == shared_ptr<PlayerEntity>());
	shared_ptr<PlayerEntity> newPlayer(new PlayerEntity(position, shipID));
	mEntities.push_back(newPlayer);
	mPlayer = newPlayer;
	return newPlayer;
}

shared_ptr<EnemyEntity> EntityManager::createEnemy(const Vec2& position, float speed)
{
	shared_ptr<EnemyEntity> newEnemy(new EnemyEntity(position, speed));
	mEntities.push_back(newEnemy);
	newEnemy->_setupParts(); // this guarantees that parts are updated after their parent
	return newEnemy;
}

shared_ptr<EnemyPartEntity> EntityManager::createEnemyPart(const Vec2& position, const string& texture, int health, shared_ptr<EnemyEntity> parent)
{
	shared_ptr<EnemyPartEntity> newPart(new EnemyPartEntity(position, texture, health, parent));
	mEntities.push_back(newPart);
	return newPart;
}

shared_ptr<BulletEntity> EntityManager::createBullet(bool friendly)
{
	shared_ptr<BulletEntity> newBullet(new BulletEntity);
	(friendly ? mPlayerBullets : mNonPlayerBullets).push_back(newBullet);
	return newBullet;
}

shared_ptr<PlayerEntity> EntityManager::getPlayer() const
{
	return mPlayer;
}

void EntityManager::destroyEntity(shared_ptr<Entity> ent)
{
	if (dynamic_pointer_cast<BulletEntity>(ent) != shared_ptr<BulletEntity>())
	{
		mPlayerBullets.erase(remove(mPlayerBullets.begin(), mPlayerBullets.end(), ent), mPlayerBullets.end());
		mNonPlayerBullets.erase(remove(mNonPlayerBullets.begin(), mNonPlayerBullets.end(), ent), mNonPlayerBullets.end());
	}
	else
	{
		mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
	}
}

void EntityManager::updateAll(float dt)
{
	vector<shared_ptr<Entity>> currentEntities = mEntities;

	// Update each entity
	for (auto i = currentEntities.begin(); i != currentEntities.end(); ++i)
		(*i)->update(dt);
	for (auto i = mPlayerBullets.begin(); i != mPlayerBullets.end(); ++i)
		(*i)->update(dt);
	for (auto i = mNonPlayerBullets.begin(); i != mNonPlayerBullets.end(); ++i)
		(*i)->update(dt);

	// Detect collisions between entities
	for (auto i = currentEntities.begin(); i != currentEntities.end(); ++i)
	{
		// Entity on Entity
		for (auto j = i + 1; j != currentEntities.end(); ++j)
		{
			if (Collision::BoundingBoxTest((*i)->getSprite(), (*j)->getSprite()))
			{
				(*i)->onCollision(*j);
				(*j)->onCollision(*i);
			}
		}

		// Player Bullet on Entity
		if (*i != mPlayer)
		{
			for (auto j = mPlayerBullets.begin(); j != mPlayerBullets.end(); ++j)
			{
				if ((*j)->isActive())
				{
					if (Collision::BoundingBoxTest((*i)->getSprite(), (*j)->getSprite()))
					{
						(*i)->onCollision(*j);
						(*j)->onCollision(*i);
					}
				}
			}
		}
	}

	// Non-player bullets on Player
	for (auto i = mNonPlayerBullets.begin(); i != mNonPlayerBullets.end(); ++i)
	{
		if ((*i)->isActive())
		{
			if (Collision::PixelPerfectTest((*i)->getSprite(), mPlayer->getSprite()))
			{
				(*i)->onCollision(mPlayer);
				mPlayer->onCollision(*i);
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

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getPlayerBulletsBegin()
{
	return mPlayerBullets.begin();
}

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getPlayerBulletsEnd()
{
	return mPlayerBullets.end();
}

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getNonPlayerBulletsBegin()
{
	return mNonPlayerBullets.begin();
}

vector<shared_ptr<BulletEntity>>::iterator EntityManager::getNonPlayerBulletsEnd()
{
	return mNonPlayerBullets.end();
}
