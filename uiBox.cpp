#include "uiBox.h"
#include "game.h"

UIBox::UIBox(const Vector2D pos, const float width, const float height, const int fontSize, const Vector2D textOffset, const std::string &text, Sprite *sprite)
{
	mPosition = pos;
	mWidth = width;
	mHeight = height;
	mFontSize = fontSize;
	mTextOffset = textOffset;
	mText = text;
	mBackground = sprite;

	mScale = Vector2D(1, 1);
	mPadding = 1.3f; //debug value

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
	mPadding = other.mPadding;
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
}

bool UIBox::contains(Vector2D pos)
{
	return pos.getX() > mPosition.getX() && pos.getX() < mPosition.getX() + mWidth && pos.getY() > mPosition.getY() && pos.getY() < mPosition.getY() + mHeight;
}

void UIBox::draw(Font &font, Color &color)
{
	//TODO: fix box backgrounds and also do null check for mBackground
	//Game::getInstance()->getGraphics().drawScale(mPosition, *mBackground, mScale);
	Game::getInstance()->getGraphics().writeText(mPosition + mTextOffset, mFontSize, font, color, mText);
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
	int textWidth = Game::getInstance()->getDefaultFont().getWidth(mText, mFontSize);
	int textHeight = 12; //debug
	mScale = Vector2D((textWidth * mPadding) / mBackground->getWidth(), (textHeight * mPadding) / mBackground->getHeight());
	//mTextOffset = Vector2D(mBackground->getWidth() * mPadding / 2, mBackground->getHeight() * mPadding / 2);
	int i = 0;
}
