#pragma once
#include "CityCard.h"

class PlayerCard : public CityCard
{
public:
	PlayerCard(Vector2D pos, Sprite *s, City *city);
	virtual ~PlayerCard();
};