// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef EnemyEntity_h__
#define EnemyEntity_h__

#include "ShipEntity.h"

class EnemyEntity : public ShipEntity, public enable_shared_from_this<EnemyEntity>
{
public:
	EnemyEntity(const Vec2& position, float speed);
	virtual ~EnemyEntity();

	// Sub-parts
	// TODO

	// Inherited from ShipEntity
	virtual void damage(const Vec2& direction, uint damageTaken);
	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);
	virtual void onCollision(shared_ptr<Entity> other);
	virtual sf::Sprite& getSprite();

private:
	float mSpeed;

	sf::Texture mTexture; // TODO handle this in a texture manager
	sf::Sprite mSprite;

};

#endif // EnemyEntity_h__

