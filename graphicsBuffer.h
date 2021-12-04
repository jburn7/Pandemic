#pragma once
#include "SFML\Graphics\Texture.hpp"
#include <string>
#include "color.h"

class GraphicsBuffer : public Trackable
{
	friend class Color;
	friend class GraphicsSystem;
public:
	GraphicsBuffer();

	GraphicsBuffer(Color &color, int w, int h);

	GraphicsBuffer(std::string &resource);

	~GraphicsBuffer();

	void create(int w, int h);

	void setHeight(int h);

	void setWidth(int w);

	int getHeight();

	int getWidth();
protected:
	int mWidth, mHeight;

	sf::Texture mBitmap;
};