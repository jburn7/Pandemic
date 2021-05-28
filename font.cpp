#include "font.h"

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

void Font::loadFont(std::string &fontPath, int fs)
{
	mFont.loadFromFile(fontPath);
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
