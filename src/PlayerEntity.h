// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef PlayerEntity_h__
#define PlayerEntity_h__

#include "Entity.h"

class PlayerEntity : public Entity
{
public:
	PlayerEntity(const Vec2& position);
	virtual ~PlayerEntity();

	virtual void update(float dt);
	virtual void render();

private:
};

#endif // PlayerEntity_h__
