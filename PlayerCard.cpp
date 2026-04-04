#include "PlayerCard.h"
#include "game.h"

PlayerCard::PlayerCard(Vector2D pos, const GraphicsBuffer& graphicsBuffer, City *city) : CityCard(pos, graphicsBuffer, city)
{
	mCardType = PlayerCardType::CITY;
}

PlayerCard::~PlayerCard()
{
	CityCard::~CityCard();
}

PlayerCardType PlayerCard::getCardType() const
{
	return mCardType;
}
