/*
	Ludum Dare 28 Entry
	Copyright (c) David Avedissian 2013
*/
#ifndef Renderer_h__
#define Renderer_h__

class Renderer : public Singleton<Renderer>
{
public:
	Renderer(uint width, uint height, bool fullscreen);
	~Renderer();

	void render();
	void processEvents();

	sf::Window& getWindow();

private:
	sf::Window mWindow;

};

#endif // Renderer_h__
