#include "InfectionCard.h"
#include "City.h"

InfectionCard::InfectionCard(const Vector2D &pos, Sprite *sprite, City *city) : CityCard(pos, sprite, city)
{
	sprite->setColor(Color(5, 200, 10));
}

InfectionCard::~InfectionCard()
{
	CityCard::~CityCard();
}
