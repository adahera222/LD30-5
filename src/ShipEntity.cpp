// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "ShipEntity.h"

ShipEntity::ShipEntity(const Vec2& position, int health) :
	Entity(position),
	mHealth(health)
{
}

ShipEntity::~ShipEntity()
{
}

void ShipEntity::damage(const Vec2& direction, uint damageTaken)
{
	mHealth -= damageTaken;
}