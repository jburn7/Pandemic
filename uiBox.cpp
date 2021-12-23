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
	mPadding = 6.f; //debug value

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
	//TODO: fix box backgrounds
	if(mBackground)
	{
		Game::getInstance()->getGraphics().drawScale(mPosition, *mBackground, mScale);
	}
	Game::getInstance()->getGraphics().writeText(mPosition + mTextOffset, mFontSize, font, color, mText);
}

void UIBox::move(const Vector2D & delta)
{
	setPosition(mPosition + delta);
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
	int textHeight = Game::getInstance()->getDefaultFont().getHeight(mText, mFontSize); //TODO: give a font pointer in init
	// TODO: add a second "padding" background that has the same position at background but is just a solid color and is bigger in size by [padding] pixels
	mScale = Vector2D((float)(textWidth) / (float)mBackground->getWidth(), (float)(textHeight) / (float)mBackground->getHeight());
	float backgroundWidth = mBackground->getWidth() * mScale.getX();
	float backgroundHeight = mBackground->getHeight() * mScale.getY();
	mTextOffset = Vector2D(backgroundWidth / 2, backgroundHeight / 2) - Vector2D((float)textWidth / 2, (float)textHeight);
}
