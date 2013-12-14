// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#include "Common.h"
#include "Game.h"

int main(int argc, char** argv)
{
	return Game::run();
}

// Win32 Main
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(__argc, __argv);
}