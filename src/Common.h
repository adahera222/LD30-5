// Ludum Dare 28 Entry
// Copyright (c) David Avedissian 2013
#ifndef Common_h__
#define Common_h__

// Standard Library
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <list>
using namespace std;

// SFML
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// Common typedefs
typedef unsigned int uint;
typedef unsigned long ulong;
typedef sf::Vector2f Vec2;

#endif // Common_h__