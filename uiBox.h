#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"

class UIBox : public Trackable
{
public:
	UIBox(const Vector2D pos, const float width, const float height, const int fontSize, const Vector2D textOffset, const std::string &text, Sprite *sprite);
	UIBox(const UIBox &other);
	~UIBox();

	bool contains(Vector2D pos);

	void draw(Font &font, Color &color);

	void move(const Vector2D &delta);

	//setters
	void setPosition(Vector2D pos);
	void setText(const std::string &text);
private:
	void resizeBackground();

	Vector2D mPosition;
	float mWidth, mHeight;
	Vector2D mScale;
	int mFontSize;
	std::string mText;
	Sprite *mBackground;
	float mPadding;
	Vector2D mTextOffset;
	Color mColor;
};