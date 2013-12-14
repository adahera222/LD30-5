// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Renderer_h__
#define Renderer_h__

#include "Singleton.h"

class Renderer : public Singleton<Renderer>
{
public:
	Renderer(uint width, uint height, bool fullscreen);
	~Renderer();

	void processEvents();
	void render();

	sf::RenderWindow& getWindow();

private:
	sf::RenderWindow mWindow;

};

#endif // Renderer_h__
