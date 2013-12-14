// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef PlayerEntity_h__
#define PlayerEntity_h__

#include "Entity.h"

struct Shield
{
	sf::Sprite sprite;
	float visibility;
	float rotation;
};

class PlayerEntity : public Entity
{
public:
	PlayerEntity(const Vec2& position, uint shipID);
	virtual ~PlayerEntity();

	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);

	void _hitShield(const Vec2& direction);

private:
	uint mShipID;

	sf::Texture mShipTexture;
	sf::Texture mShieldTexture;

	sf::Sprite mShipSprite;
	list<Shield> mShieldSprites;
};

#endif // PlayerEntity_h__