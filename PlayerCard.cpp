#include "PlayerCard.h"
#include "game.h"

PlayerCard::PlayerCard(Vector2D pos, Sprite *s, City *city) : CityCard(pos, s, city)
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
