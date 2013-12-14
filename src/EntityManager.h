// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EntityManager_h__
#define EntityManager_h__

#include "Singleton.h"
#include "Entity.h"
#include "PlayerEntity.h"
#include "EnemyEntity.h"
#include "BulletEntity.h"

class EntityManager : public Singleton<EntityManager>
{
public:
	EntityManager();
	~EntityManager();

	shared_ptr<PlayerEntity> createPlayer(const Vec2& position, uint shipID);
	shared_ptr<EnemyEntity> createEnemy(const Vec2& position, float speed);
	shared_ptr<BulletEntity> createBullet();
	void destroyEntity(shared_ptr<Entity> ent);

	void updateAll(float dt);

	vector<shared_ptr<Entity>>::iterator getEntitiesBegin();
	vector<shared_ptr<Entity>>::iterator getEntitiesEnd();
	vector<shared_ptr<BulletEntity>>::iterator getBulletEntitiesBegin();
	vector<shared_ptr<BulletEntity>>::iterator getBulletEntitiesEnd();

private:
	vector<shared_ptr<Entity>> mEntities;
	vector<shared_ptr<BulletEntity>> mBulletEntities; // bullets are stored seperately for collision detection

};

#endif // EntityManager_h__
