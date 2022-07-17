#include "uiBox.h"
#include "game.h"
#include "Glyph.h"

UIBox::UIBox(const Vector2D pos, const int fontSize, const Vector2D textOffset, const Color& color, const float padding, const std::string &text, Sprite *sprite, Sprite *background, Font *font)
{
	mPosition = pos;
	mFontSize = fontSize;
	mTextOffset = textOffset;
	mColor = color;
	mText = text;
	mBackground = sprite;
	mPaddingBackground = background;
	if(mFont)
	{
		mFont = font;
	}
	else
	{
		mFont = &Game::getInstance()->getDefaultFont();
	}

	mScale = Vector2D(1, 1);
	mPaddingBackgroundScale = Vector2D(1, 1);
	mPaddingAmount = padding; //debug value

	setIsGuiLayer(true);

	mWidth = 0;
	mHeight = 0;

	resizeBackground();
}

UIBox::UIBox(const UIBox &other)
{
	mPosition = other.mPosition;
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mFontSize = other.mFontSize;
	mText = other.mText;
	if(other.mBackground)
	{
		mBackground = new Sprite(*other.mBackground);
		delete other.mBackground;
	}
	else
	{
		mBackground = nullptr;
	}
	if(other.mPaddingBackground)
	{
		mBackground = new Sprite(*other.mPaddingBackground);
		delete other.mPaddingBackground;
	}
	else
	{
		mBackground = nullptr;
	}
	mPaddingAmount = other.mPaddingAmount;
	mTextOffset = other.mTextOffset;
	mColor = other.mColor;
	mScale = other.mScale;

	resizeBackground();
}

UIBox::~UIBox()
{
	if(mBackground)
	{
		delete mBackground;
		mBackground = NULL;
	}
	if(mPaddingBackground)
	{
		delete mPaddingBackground;
		mPaddingBackground = NULL;
	}

	mFont = nullptr;
}

bool UIBox::contains(Vector2D pos)
{
	return pos.getX() > mPosition.getX() && pos.getX() < mPosition.getX() + mWidth && pos.getY() > mPosition.getY() && pos.getY() < mPosition.getY() + mHeight;
}

void UIBox::draw()
{
	if(mPaddingBackground)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mPaddingBackground, 0, mPaddingBackgroundScale);
	}
	if(mBackground)
	{
		Game::getInstance()->getGraphics().draw(mPosition + Vector2D(mTextOffset.getX(), -mTextOffset.getY()), *mBackground, 0, mScale);
	}
	Game::getInstance()->getGraphics().writeText(mPosition + mTextOffset, mFontSize, *mFont, mColor, mText);
}

void UIBox::move(const Vector2D & delta)
{
	setPosition(mPosition + delta);
}

void UIBox::resizeToFitWidth(const float boundsWidth)
{
	// TODO: move down to accomodate for shrinkage?
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
		mFontSize /= ratio;
	}
}

void UIBox::setPosition(Vector2D pos)
{
	mPosition = pos;
}

void UIBox::setText(const std::string & text)
{
	mText = text;
	resizeBackground();
}

void UIBox::resizeBackground()
{
	//pad the text with a background that gives in both dimensions
	int textWidth = mFont->getWidth(mText, mFontSize);
	int textHeight = mFont->getHeight(mText, mFontSize);
	mScale = Vector2D((float)(textWidth) / (float)mBackground->getWidth(), (float)(textHeight) / (float)mBackground->getHeight());
	float backgroundWidth = std::floor(mBackground->getWidth() * mScale.getX());
	float backgroundHeight = std::floor(mBackground->getHeight() * mScale.getY());
	if(mPaddingBackground)
	{
		mPaddingBackgroundScale = Vector2D((textWidth + mPaddingAmount) / (mPaddingBackground->getWidth()), (textHeight + mPaddingAmount) / (mPaddingBackground->getHeight()));
		backgroundWidth = std::floor(mBackground->getWidth() * mPaddingBackgroundScale.getX());
		backgroundHeight = std::floor(mBackground->getHeight() * mPaddingBackgroundScale.getY());
	}
	mWidth = backgroundWidth;
	mHeight = backgroundHeight;
	mTextOffset = Vector2D(backgroundWidth / 2, backgroundHeight / 2) - Vector2D((float)textWidth / 2, (float)textHeight);
}
