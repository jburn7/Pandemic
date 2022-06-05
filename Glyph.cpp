#include "Glyph.h"

Glyph::Glyph(const sf::Glyph &glyph) : mGlyph(glyph)
{
}

Glyph::~Glyph()
{

}

float Glyph::getAdvance() const
{
	return mGlyph.advance;
}
