#pragma once
#include "SFML\Graphics\Color.hpp"
#include "Trackable.h"

class GraphicsBuffer;

class Color : public Trackable
{
	friend class GraphicsSystem;
public:
	Color();
	Color(int red, int green, int blue);
	~Color();
protected:
	sf::Color mColor;
};