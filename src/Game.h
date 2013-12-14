// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef App_h__
#define App_h__

// Main class that handles an application
class Game
{
public:
	static int run();
	static float getTimeRate();
	
	// Resolution - this is fixed
	static const uint SCREEN_WIDTH = 640;
	static const uint SCREEN_HEIGHT = 480;

};

#endif // App_h__