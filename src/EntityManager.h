// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EntityManager_h__
#define EntityManager_h__

#include "Singleton.h"
#include "Entity.h"
#include "PlayerEntity.h"
#include "BulletEntity.h"

class EntityManager : public Singleton<EntityManager>
{
public:
	EntityManager();
	~EntityManager();

	PlayerEntity* createPlayer(const Vec2& position, uint shipID);
	BulletEntity* createBullet();
	void destroyEntity(Entity* ent);

	void updateAll(float dt);

	vector<Entity*>::iterator getEntitiesBegin();
	vector<Entity*>::iterator getEntitiesEnd();
	vector<BulletEntity*>::iterator getBulletEntitiesBegin();
	vector<BulletEntity*>::iterator getBulletEntitiesEnd();

private:
	vector<Entity*> mEntities;
	vector<BulletEntity*> mBulletEntities; // bullets are stored seperately for collision detection

};

#endif // EntityManager_h__
