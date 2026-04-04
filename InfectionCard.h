#pragma once
#include "CityCard.h"

class City;

class InfectionCard : public CityCard
{
public:
	InfectionCard(const Vector2D &pos, const GraphicsBuffer& graphicsBuffer, City *city);

	virtual ~InfectionCard();
};