#pragma once
#include "SFML\Graphics\Color.hpp"
#include "Trackable.h"

class GraphicsBuffer;

class Color : public Trackable
{
	friend class GraphicsSystem;
public:
	Color(int red = 255, int green = 255, int blue = 255, int alpha = 255);
	~Color();
protected:
	sf::Color mColor;
};