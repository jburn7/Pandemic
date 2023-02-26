#include "uiBox.h"
#include "game.h"
#include "Glyph.h"

UIBox::UIBox(const Vector2D pos, const int fontSize, const Color& color, const std::string &text, Font *font)
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

	mWidth = 0;
	mHeight = 0;
}

UIBox::UIBox(const UIBox &other)
{
	mPosition = other.mPosition;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFontSize = other.mFontSize;
	mText = other.mText;
	mColor = other.mColor;
	mScale = other.mScale;
}

UIBox::~UIBox()
{
	mFont = nullptr;
}

bool UIBox::contains(Vector2D pos)
{
	return pos.getX() > mPosition.getX() && pos.getX() < mPosition.getX() + mWidth && pos.getY() > mPosition.getY() && pos.getY() < mPosition.getY() + mHeight;
}

void UIBox::draw()
{
	Game::getInstance()->getGraphics().writeText(mPosition + Vector2D((float)mOutline.thickness, (float)mOutline.thickness), mFontSize, *mFont, mColor, mText, mOutline, mScale);
}

void UIBox::resizeToFitWidth(const float boundsWidth)
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
}

void UIBox::setText(const std::string & text)
{
	mText = text;
}
