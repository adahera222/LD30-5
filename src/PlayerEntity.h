// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef PlayerEntity_h__
#define PlayerEntity_h__

#include "DameageableEntity.h"

struct Shield
{
	sf::Sprite sprite;
	float visibility;
	float rotation;
};

class PlayerEntity : public DamageableEntity, public enable_shared_from_this<PlayerEntity>
{
public:
	PlayerEntity(const Vec2& position, uint shipID);
	virtual ~PlayerEntity();

	bool inBulletTime() const;

	// Inherited from ShipEntity
	virtual void damage(const Vec2& direction, uint damageTaken);
	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);
	virtual void onCollision(shared_ptr<Entity> other);
	virtual sf::Sprite& getSprite();

	void _hitShield(const Vec2& direction);

private:
	uint mShipID;

	// Bullet Time
	bool mBulletTime;

	// Shooting
	vector<Vec2> mGunPoints;
	sf::Clock mGunTimer;

	// Velocity
	Vec2 mVelocity;

	// Rendering
	sf::Texture mShipTexture;
	sf::Texture mShieldTexture;

	sf::Sprite mShipSprite;
	list<Shield> mShieldSprites;
};

#endif // PlayerEntity_h__
