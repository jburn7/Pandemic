#include "InfectionCard.h"
#include "City.h"

InfectionCard::InfectionCard(const Vector2D &pos, const GraphicsBuffer& graphicsBuffer, City *city) : CityCard(pos, graphicsBuffer, city)
{
	setColor(Color(5, 200, 10));
}

InfectionCard::~InfectionCard()
{
	CityCard::~CityCard();
}
