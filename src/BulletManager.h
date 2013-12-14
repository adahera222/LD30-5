// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef BulletManager_h__
#define BulletManager_h__

#include "Singleton.h"
#include "BulletEntity.h"

enum BulletType
{
	BT_PLAYER_COMMON,
	BT_ENEMY_COMMON
};

class BulletManager : public Singleton<BulletManager>
{
public:
	BulletManager();
	~BulletManager();

	void spawn(const Vec2& position, const Vec2& velocity, Entity* parent, BulletType type);

private:
	map<BulletType, sf::Texture> mBulletTextures;
	list<BulletEntity*> mBulletCache;

};

#endif // BulletManager_h__
