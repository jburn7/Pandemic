#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"

class UIBox : public Trackable
{
public:
	UIBox(const Vector2D pos, const float width, const float height, const int fontSize, const Vector2D textOffset, const Color &color, const float padding, const std::string &text, Sprite *sprite, Sprite *background);
	UIBox(const UIBox &other);
	~UIBox();

	bool contains(Vector2D pos);

	void draw(Font &font);

	void move(const Vector2D &delta);

	//setters
	void setPosition(Vector2D pos);
	void setText(const std::string &text);
private:
	void resizeBackground();

	Vector2D mPosition;
	float mWidth, mHeight;
	Vector2D mScale, mPaddingBackgroundScale;
	int mFontSize;
	std::string mText;
	Sprite *mBackground;
	Sprite *mPaddingBackground;
	float mPaddingAmount;
	Vector2D mTextOffset;
	Color mColor;
};