#include "textBox.h"
#include "game.h"
#include "Glyph.h"

TextBox::TextBox(const Vector2D pos, const int fontSize, const Color& color, const std::string &text, Font *font)
{
	mPosition = pos;
	mFontSize = fontSize;
	mColor = color;
	mText = text;
	if(mFont)
	{
		mFont = font;
	}
	else
	{
		mFont = &Game::getInstance()->getDefaultFont();
	}

	setIsGuiLayer(true);

	mGraphicsText = sf::Text(text, mFont->mFont, fontSize);
	adjustBoundsForText();
}

TextBox::TextBox(const TextBox &other)
{
	mPosition = other.mPosition;
	mGraphicsText = other.mGraphicsText;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFontSize = other.mFontSize;
	mText = other.mText;
	mColor = other.mColor;
	mScale = other.mScale;
}

TextBox::~TextBox()
{
	mFont = nullptr;
}

bool TextBox::contains(Vector2D pos) const
{
	return pos.getX() > mPosition.getX() && pos.getX() < mPosition.getX() + mWidth && pos.getY() > mPosition.getY() && pos.getY() < mPosition.getY() + mHeight;
}

void TextBox::draw() const
{
	Game::getInstance()->getGraphics().writeText(mPosition + Vector2D((float)mOutline.thickness, (float)mOutline.thickness), mFontSize, *mFont, mColor, mText, mOutline, mScale);
}

void TextBox::resizeToFitWidth(const float boundsWidth)
{
	std::vector<int> whitespaceIndeces;
	std::vector<float> wordWidths;
	int index = 0;
	float totalWidth = 0.f;
	float lastWordWidth = 0.f;
	float largestWordWidth = 0.f;

	// Calculate text width and space locations
	for(char c : mText)
	{
		if(c == ' ')
		{
			whitespaceIndeces.push_back(index);
			float wordWidth = totalWidth - lastWordWidth;
			wordWidths.push_back(wordWidth);
			lastWordWidth = wordWidth;
			if(wordWidth > largestWordWidth)
			{
				largestWordWidth = wordWidth;
			}
		}

		Glyph glyph = mFont->getGlyph(c, mFontSize, false);
		totalWidth += glyph.getAdvance();
		index++;
	}

	// First, try to break on last whitespace recursively
	index = whitespaceIndeces.size() - 1;
	while(totalWidth > boundsWidth && index >= 0)
	{
		mText[whitespaceIndeces[index]] = '\n';
		totalWidth -= wordWidths[index];
		index--;
	}

	// Then if that doesn't work, shrink largest word width into boundsWidth and adjust fontSize accordingly
	if(totalWidth > boundsWidth)
	{
		float ratio = totalWidth / boundsWidth;
		mFontSize = (int)(mFontSize / ratio);
	}

	setText(mText);
}

int TextBox::getWidth() const
{
	return (int)mWidth;
}

int TextBox::getHeight() const
{
	return (int)mHeight;
}

void TextBox::setText(const std::string &text)
{
	mText = text;
	mGraphicsText.setString(text);
	adjustBoundsForText();
}

void TextBox::adjustBoundsForText()
{
	const sf::FloatRect bounds = mGraphicsText.getGlobalBounds();
	mWidth = bounds.width + bounds.left;
	mHeight = bounds.height + bounds.top;
}
