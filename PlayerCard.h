#pragma once
#include "Unit.h"
#include "City.h"

class PlayerCard : public Unit
{
public:
	PlayerCard(Vector2D pos, Sprite *s, City *city);
	virtual ~PlayerCard();

	virtual void draw();

	//setters
	void setPosition(Vector2D pos);

	// Getters
	City* getCity();
private:
	City *mpCity; //pointer to city that this player card corresponds to
	UIBox *mText;
};