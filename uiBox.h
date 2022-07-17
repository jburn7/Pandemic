#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"
#include "unit.h"

class UIBox : public Unit
{
public:
	UIBox(const Vector2D pos, const int fontSize, const Vector2D textOffset, const Color &color, const float padding, const std::string &text, Font* font = nullptr);
	UIBox(const UIBox &other);
	~UIBox();

	bool contains(Vector2D pos);

	// Can either be manually drawn if another unit owns it or draw itself if it is added to UnitManager
	virtual void draw();

	void move(const Vector2D &delta);

	void resizeToFitWidth(const float boundsWidth);

	//setters
	void setPosition(Vector2D pos);
	void setText(const std::string &text);
private:
	Font *mFont;
	Vector2D mPosition;
	float mWidth, mHeight;
	Vector2D mScale;
	int mFontSize;
	std::string mText;
	Color mColor;
};