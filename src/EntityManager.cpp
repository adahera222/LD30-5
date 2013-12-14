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

PlayerEntity* EntityManager::createPlayer(const Vec2& position)
{
	PlayerEntity* newPlayer = new PlayerEntity(position);
	mEntities.push_back(newPlayer);
	return newPlayer;
}

void EntityManager::destroyEntity(Entity* ent)
{
	assert(find(mEntities.begin(), mEntities.end(), ent) != mEntities.end());
	mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
	delete ent;
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
