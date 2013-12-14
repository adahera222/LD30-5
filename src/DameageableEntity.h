// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Ship_h__
#define Ship_h__

#include "Entity.h"

class DamageableEntity : public Entity
{
public:
	DamageableEntity(const Vec2& position, int health);
	virtual ~DamageableEntity();

	virtual void damage(const Vec2& direction, uint damageTaken);

protected:
	int mHealth;

};

#endif // Ship_h__
