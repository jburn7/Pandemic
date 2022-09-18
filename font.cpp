#include "font.h"
#include "Glyph.h"

Font::Font()
{
}

Font::Font(std::string &fontPath, int fs)
{
	loadFont(fontPath, fs);
}

Font::~Font()
{
}

void Font::loadFont(const std::string &fontPath, int fs)
{
	mFont.loadFromFile(fontPath);
}

Glyph Font::getGlyph(uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness) const
{
	return Glyph(mFont.getGlyph(codePoint, characterSize, bold, outlineThickness));
}

int Font::getWidth(const std::string message, const int fontSize)
{
	sf::Text text(message, mFont, fontSize);
	return (int)text.getGlobalBounds().width;
}

int Font::getHeight(const std::string message, const int fontSize)
{
	sf::Text text(message, mFont, fontSize);
	return (int)text.getGlobalBounds().height;
}
