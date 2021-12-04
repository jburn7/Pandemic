#pragma once
#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Graphics\Sprite.hpp"
#include "SFML\Graphics\Text.hpp"
#include "sprite.h"
#include "font.h"
#include <string>

class GraphicsSystem : public Trackable
{
	friend class InputSystem; //for polling RenderWindow Events
public:
	GraphicsSystem();

	~GraphicsSystem();

	void init(int w, int h, const std::string &title);

	void cleanup();

	void clear();

	void draw(const Vector2D &targetLoc, Sprite &sprite);

	void draw(const Vector2D &targetLoc, Sprite &sprite, double theta, const Vector2D &scale);

	void drawScale(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale);

	void flip();

	void writeText(const Vector2D &targetLoc, const int fontSize, Font &font, Color &color, std::string &message);

	//setters and getters
	void setHeight(int h);

	void setWidth(int w);

	int getHeight();

	int getWidth();
protected:
	int mWidth, mHeight;

	// TODO: add camera position so screen can scroll. Look at Untitled Final Project for example (or just straight copy/paste, that system is really simple)
	sf::RenderWindow mDisplay;
};