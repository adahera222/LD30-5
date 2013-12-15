// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "DameageableEntity.h"
#include "BulletEntity.h"
#include "EntityManager.h"

DamageableEntity::DamageableEntity(const Vec2& position, int health) :
	Entity(position),
	mHealth(health)
{
}

DamageableEntity::~DamageableEntity()
{
}

void DamageableEntity::damage(const Vec2& direction, uint damageTaken)
{
	mHealth -= damageTaken;
}

void DamageableEntity::onCollision(shared_ptr<Entity> other)
{
	shared_ptr<BulletEntity> bullet = dynamic_pointer_cast<BulletEntity>(other);
	if (bullet != shared_ptr<BulletEntity>())
	{
		// Take damage
		damage(other->getPosition() - mPosition, 5);

		// Despawn bullet
		bullet->despawn();
	}

	shared_ptr<SpecialWeapon> special = dynamic_pointer_cast<SpecialWeapon>(other);
	if (special != shared_ptr<SpecialWeapon>())
	{
		// Take (lots) of damage
		damage(other->getPosition() - mPosition, 100);
		EntityManager::inst().destroyEntity(special);
	}
}