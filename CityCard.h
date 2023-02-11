#pragma once
#include "Unit.h"

class City;
class UIBox;

class CityCard : public Unit
{
public:
	CityCard(Vector2D pos, Sprite *s, City *city);
	virtual ~CityCard();

	virtual void draw();

	virtual std::string debugDescription();

	virtual void move(const Vector2D &delta);

	//setters
	void setPosition(Vector2D pos);

	// Getters
	City* getCity();
protected:
	City *mpCity; //pointer to city that this card corresponds to
	Sprite *mpColorIndicator;
	UIBox *mText;
};