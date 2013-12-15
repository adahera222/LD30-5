// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef PlayerEntity_h__
#define PlayerEntity_h__

#include "DameageableEntity.h"

struct Shield
{
	float visibility;
	float rotation;
};

struct Lock
{
	weak_ptr<DamageableEntity> entity;
	bool hasLock;
	float lockProgress;
};

class PlayerEntity : public DamageableEntity, public enable_shared_from_this<PlayerEntity>
{
public:
	PlayerEntity(const Vec2& position, uint shipID);
	virtual ~PlayerEntity();

	bool inBulletTime() const;

	void _specialAttack();

	void _hitShield(const Vec2& direction);

	// Inherited from ShipEntity
	virtual void damage(const Vec2& direction, uint damageTaken);
	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);
	virtual void onCollision(shared_ptr<Entity> other);
	virtual sf::Sprite& getSprite();

private:
	uint mShipID;

	// Bullet Time
	bool mBulletTime;

	// Shooting
	vector<Vec2> mGunPoints;
	sf::Clock mGunTimer;

	// Velocity
	Vec2 mVelocity;

	// Shield list
	list<Shield> mShields;

	// Locks
	vector<Lock> mLocks;

	// Rendering
	sf::Texture mShipTexture;
	sf::Texture mShieldTexture;
	sf::Texture mTargetTexture;
	sf::Texture mLockingOnTexture;
	sf::Texture mLockOnTexture;
	sf::Sprite mShipSprite;
	sf::Sprite mShieldSprite;
	sf::Sprite mTargetSprite;
	sf::Sprite mLockingOnSprite;
	sf::Sprite mLockOnSprite;
};

#endif // PlayerEntity_h__
