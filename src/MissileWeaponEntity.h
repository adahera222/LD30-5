#pragma once

#include "SpecialWeaponEntity.h"
#include "DameageableEntity.h"

class MissileWeaponEntity : public SpecialWeaponEntity
{
public:
	MissileWeaponEntity(const Vec2& startingPosition, weak_ptr<DamageableEntity> target);
	virtual ~MissileWeaponEntity();

	weak_ptr<DamageableEntity> getTarget() const;

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

