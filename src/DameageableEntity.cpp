// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "DameageableEntity.h"

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