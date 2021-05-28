#pragma once
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include <string>
#include "Trackable.h"

class Font : public Trackable{
	friend class GraphicsSystem;
public:
	Font();

	Font(std::string &fontPath, int fs);

	~Font();

	void loadFont(std::string &fontPath, int fs);

	//getters
	int getWidth(const std::string message, const int fontSize);
	int getHeight(const std::string message, const int fontSize);
protected:
	sf::Font mFont;
};