#include "PlayerCard.h"
#include "game.h"

PlayerCard::PlayerCard(Vector2D pos, Sprite *s, City *city) : CityCard(pos, s, city)
{
}

PlayerCard::~PlayerCard()
{
	CityCard::~CityCard();
}
