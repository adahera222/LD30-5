// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "EntityManager.h"

EntityManager::EntityManager()
{
	// Load JSON
	ifstream file("media/enemy-detail.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(file, root);
	for (Json::ValueIterator i = root.begin(); i != root.end(); ++i)
	{
		const Json::Value& enemy = *i;

		// Fill in enemy data
		EnemyDesc enemyDesc;
		enemyDesc.name = enemy["Name"].asString();
		enemyDesc.sprite = enemy["Sprite"].asString();
		enemyDesc.health = enemy["Health"].asUInt();
		enemyDesc.speed = enemy["Speed"].asFloat();

		// Add parts
		if (enemy.isMember("Parts"))
		{
			const Json::Value& partsList = enemy.get("Parts", Json::arrayValue);
			for (Json::ValueIterator l = partsList.begin(); l != partsList.end(); ++l)
			{
				Json::Value& part = *l;

				// Fill in part data
				EnemyPartDesc partDesc;
				partDesc.sprite = part.get("Sprite", "").asString();
				partDesc.health = part.get("Health", 999999).asUInt();
				partDesc.bulletSpeed = part.get("BulletSpeed", 250.0f).asFloat();
				partDesc.bulletInterval = part.get("BulletInterval", 0.25f).asFloat();
				partDesc.position.x = part.get("Position", Json::arrayValue).get((Json::ArrayIndex)0, 0.0f).asFloat();
				partDesc.position.y = part.get("Position", Json::arrayValue).get((Json::ArrayIndex)1, 0.0f).asFloat();
				partDesc.rotation = part.get("Rotation", 0.0f).asFloat();
				partDesc.fixed = part.get("Fixed", true).asBool();
				partDesc.lockable = part.get("Lockable", false).asBool();

				enemyDesc.parts.push_back(partDesc);
			}
		}

		// Add the new enemy
		mEnemyDescs.insert(make_pair(enemyDesc.name, enemyDesc));
	}
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

shared_ptr<EnemyEntity> EntityManager::createEnemy(const Vec2& position, const string& name)
{
	shared_ptr<EnemyEntity> newEnemy(new EnemyEntity(position, mEnemyDescs[name]));
	mEntities.push_back(newEnemy);

	// This guarantees that parts are updated after their parent
	newEnemy->_setupParts(); 

	return newEnemy;
}

shared_ptr<EnemyPartEntity> EntityManager::createEnemyPart(EnemyPartDesc& partDesc, shared_ptr<EnemyEntity> parent)
{
	shared_ptr<EnemyPartEntity> newPart(new EnemyPartEntity(partDesc, parent));
	mEntities.push_back(newPart);
	return newPart;
}

shared_ptr<BulletEntity> EntityManager::createBullet(bool friendly)
{
	shared_ptr<BulletEntity> newBullet(new BulletEntity);
	(friendly ? mPlayerBullets : mNonPlayerBullets).push_back(newBullet);
	return newBullet;
}

shared_ptr<MissileSavloWeapon> EntityManager::createMissileWeapon(const Vec2& startingPosition, weak_ptr<DamageableEntity> target)
{
	shared_ptr<MissileSavloWeapon> newMissileWeapon(new MissileSavloWeapon(startingPosition, target));
	mSpecialWeapons.push_back(newMissileWeapon);
	return newMissileWeapon;
}

shared_ptr<PlayerEntity> EntityManager::getPlayer() const
{
	return mPlayer;
}

void EntityManager::destroyEntity(shared_ptr<Entity> ent)
{
	shared_ptr<EnemyEntity> enemy = dynamic_pointer_cast<EnemyEntity>(ent);
	if (enemy != shared_ptr<EnemyEntity>())
		enemy->_onDestroy();
	mSpecialWeapons.erase(remove(mSpecialWeapons.begin(), mSpecialWeapons.end(), ent), mSpecialWeapons.end());
	mNonPlayerBullets.erase(remove(mNonPlayerBullets.begin(), mNonPlayerBullets.end(), ent), mNonPlayerBullets.end());
	mPlayerBullets.erase(remove(mPlayerBullets.begin(), mPlayerBullets.end(), ent), mPlayerBullets.end());
	mEntities.erase(remove(mEntities.begin(), mEntities.end(), ent), mEntities.end());
}

bool isOutside(const Vec2& position)
{
	float padding = 150.0f;
	return position.x < -padding || position.x >(Game::SCREEN_WIDTH + padding) ||
		position.y < -padding || position.y >(Game::SCREEN_HEIGHT + padding);
}

void EntityManager::updateAll(float dt)
{
	vector<shared_ptr<Entity>> currentEntities = mEntities;
	vector<shared_ptr<SpecialWeapon>> specialWeapons = mSpecialWeapons;

	// Update each entity
	for (auto i = currentEntities.begin(); i != currentEntities.end(); ++i)
	{
		(*i)->update(dt);
		if (isOutside((*i)->getPosition()))
			destroyEntity(*i);
	}
	for (auto i = mPlayerBullets.begin(); i != mPlayerBullets.end(); ++i)
	{
		(*i)->update(dt);
		if (isOutside((*i)->getPosition()))
			(*i)->despawn();
	}
	for (auto i = mNonPlayerBullets.begin(); i != mNonPlayerBullets.end(); ++i)
	{
		(*i)->update(dt);
		if (isOutside((*i)->getPosition()))
			(*i)->despawn();
	}
	for (auto i = specialWeapons.begin(); i != specialWeapons.end(); ++i)
	{
		(*i)->update(dt);
		if (isOutside((*i)->getPosition()))
			destroyEntity(*i);
	}

	// Detect collisions between entities
	for (auto i = currentEntities.begin(); i != currentEntities.end(); ++i)
	{
		// Entity on Entity
		for (auto j = i + 1; j != currentEntities.end(); ++j)
		{
			if (Collision::PixelPerfectTest((*i)->getSprite(), (*j)->getSprite()))
			{
				(*i)->onCollision(*j);
				(*j)->onCollision(*i);
			}
		}

		if (*i != mPlayer)
		{
			// Player Bullet on Entity
			for (auto j = mPlayerBullets.begin(); j != mPlayerBullets.end(); ++j)
			{
				if ((*j)->isActive())
				{
					if (Collision::PixelPerfectTest((*i)->getSprite(), (*j)->getSprite()))
					{
						(*i)->onCollision(*j);
						(*j)->onCollision(*i);
					}
				}
			}
		}
	}

	// Special Weapons on Targets
	for (auto i = specialWeapons.begin(); i != specialWeapons.end(); ++i)
	{
		shared_ptr<MissileSavloWeapon> missile = dynamic_pointer_cast<MissileSavloWeapon>(*i);
		if (missile != shared_ptr<MissileSavloWeapon>())
		{
			shared_ptr<DamageableEntity> target = missile->getTarget().lock();
			if (target != shared_ptr<DamageableEntity>())
			{
				if (Collision::PixelPerfectTest((*i)->getSprite(), target->getSprite()))
				{
					(*i)->onCollision(target);
					target->onCollision(*i);
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

vector<shared_ptr<SpecialWeapon>>::iterator EntityManager::getSpecialWeaponsBegin()
{
	return mSpecialWeapons.begin();
}

vector<shared_ptr<SpecialWeapon>>::iterator EntityManager::getSpecialWeaponsEnd()
{
	return mSpecialWeapons.end();
}
