#pragma once
#include "Unit.h"

class City;
class TextBox;

class CityCard : public Unit
{
public:
	CityCard(Vector2D pos, const GraphicsBuffer& graphicsBuffer, City *city);
	virtual ~CityCard();

	virtual void draw() const override;

	virtual std::string debugDescription();

	virtual void move(const Vector2D &delta);

	//setters
	void setPosition(Vector2D pos);

	// Getters
	City* getCity();
protected:
	City *mpCity; //pointer to city that this card corresponds to
	// TODO: A (major) refactor could be to store Sprite and Shape inside a new Drawable class, further reinventing the SFML wheel
	Sprite *mpColorIndicator;
	Shape mColorIndicatorShape;
	TextBox *mText;
};