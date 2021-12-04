#pragma once
#include "CityCard.h"

class City;

class InfectionCard : public CityCard
{
public:
	InfectionCard(Vector2D &pos, Sprite *sprite, City *city);

	virtual ~InfectionCard();
};