#pragma once
#include "Vector2D.h"
#include "sprite.h"
#include "font.h"
#include "unit.h"

class TextBox : public Unit
{
public:
	TextBox(const Vector2D pos, const int fontSize, const Color &color, const std::string &text, Font* font = nullptr);
	TextBox(const TextBox &other);
	~TextBox();

	bool contains(Vector2D pos) const;

	// Can either be manually drawn if another unit owns it or draw itself if it is added to UnitManager
	virtual void draw() const override;

	void resizeToFitWidth(const float boundsWidth);

	//setters
	void setText(const std::string &text);
private:
	void adjustBoundsForText();

	Font *mFont;
	float mWidth, mHeight;
	int mFontSize;
	std::string mText;
	Color mColor;
	// Don't love breaking the library abstraction, but we need a way to readily get the width and height of text, as it is not set directly
	sf::Text mGraphicsText;
};