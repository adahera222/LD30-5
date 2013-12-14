// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EntityManager_h__
#define EntityManager_h__

#include "Singleton.h"
#include "Entity.h"
#include "PlayerEntity.h"

class EntityManager : public Singleton<EntityManager>
{
public:
	EntityManager();
	~EntityManager();

	PlayerEntity* createPlayer(const Vec2& position, uint shipID);
	void destroyEntity(Entity* ent);

	void updateAll(float dt);

	vector<Entity*>::iterator getEntitiesBegin();
	vector<Entity*>::iterator getEntitiesEnd();
	uint getEntitiesCount() const;

private:
	vector<Entity*> mEntities;

};

#endif // EntityManager_h__
