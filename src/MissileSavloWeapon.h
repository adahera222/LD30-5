#pragma once

#include "SpecialWeapon.h"
#include "DameageableEntity.h"

class MissileSavloWeapon : public SpecialWeapon
{
public:
	MissileSavloWeapon(const Vec2& startingPosition, weak_ptr<DamageableEntity> target);
	virtual ~MissileSavloWeapon();

	// Inherited from SpecialWeapon
	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);
	virtual void onCollision(shared_ptr<Entity> other);
	virtual sf::Sprite& getSprite();

private:
	weak_ptr<DamageableEntity> mTarget;

	// Rendering
	sf::Texture mMissileTexture;
	sf::Sprite mMissileSprite;

};

