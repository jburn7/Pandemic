#pragma once
#include "SFML\Graphics\Glyph.hpp"
#include "font.h"
#include "Trackable.h"

class Glyph : public Trackable
{
	friend class Font;

public:
	~Glyph();

	// Getters
	float getAdvance() const;
private:
	Glyph(const sf::Glyph &glyph);

	// Members
	const sf::Glyph mGlyph;
};