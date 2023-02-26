#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"
#include "unit.h"

class UIBox : public Unit
{
public:
	UIBox(const Vector2D pos, const int fontSize, const Color &color, const std::string &text, Font* font = nullptr);
	UIBox(const UIBox &other);
	~UIBox();

	bool contains(Vector2D pos);

	// Can either be manually drawn if another unit owns it or draw itself if it is added to UnitManager
	virtual void draw();

	void resizeToFitWidth(const float boundsWidth);

	//setters
	void setText(const std::string &text);
private:
	Font *mFont;
	float mWidth, mHeight;
	int mFontSize;
	std::string mText;
	Color mColor;
};