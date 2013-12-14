// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Entity_h__
#define Entity_h__

class Entity
{
public:
	Entity(const Vec2& position);
	virtual ~Entity();

	virtual void update(float dt);
	virtual void render(sf::RenderWindow& window);

	void setPosition(const Vec2& position);
	const Vec2& getPosition() const;

protected:
	Vec2 mPosition;

};

#endif // Entity_h__
