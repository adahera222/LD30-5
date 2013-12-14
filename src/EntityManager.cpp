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

void EntityManager::destroyEntity(Entity* ent)
{
	assert(find(mEntities.begin(), mEntities.end(), ent) != mEntities.end());
	mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
	delete ent;
}

void EntityManager::updateAll(float dt)
{
	for (auto i = mEntities.begin(); i != mEntities.end(); ++i)
		(*i)->update(dt);
}

vector<Entity*>::iterator EntityManager::getEntitiesBegin()
{
	return mEntities.begin();
}

vector<Entity*>::iterator EntityManager::getEntitiesEnd()
{
	return mEntities.end();
}

uint EntityManager::getEntitiesCount() const
{
	return mEntities.size();
}
