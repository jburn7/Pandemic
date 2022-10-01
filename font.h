#pragma once
#include "SFML\Graphics\Font.hpp"
#include "SFML\Graphics\Text.hpp"
#include <string>
#include "Trackable.h"

class Glyph;

class Font : public Trackable {
	friend class GraphicsSystem;
public:
	Font();

	Font(std::string &fontPath, int fs);

	~Font();

	void loadFont(const std::string &fontPath, int fs);

	Glyph getGlyph(uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness = 0) const;

	float getUnderlineSpacing(const int fontSize) const;

	//getters
	int getWidth(const std::string message, const int fontSize);
	int getHeight(const std::string message, const int fontSize);
protected:
	sf::Font mFont;
};