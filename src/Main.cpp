/*
	Ludum Dare 28 Entry
	Copyright (c) David Avedissian 2013
*/
#include "Common.h"
#include "Main.h"

int main(int argc, char** argv)
{
	sf::Window window(sf::VideoMode(800, 600), "LD28");

	// Run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}

// Win32 Main
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(__argc, __argv);
}