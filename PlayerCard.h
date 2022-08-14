#pragma once
#include "CityCard.h"

enum PlayerCardType
{
	CITY_CARD,
	EPIDEMIC_CARD,
	EVENT_CARD // TODO: add events
};

class PlayerCard : public CityCard
{
public:
	PlayerCard(Vector2D pos, Sprite *s, City *city);
	virtual ~PlayerCard();
	
	// Getters
	PlayerCardType getCardType() const;
protected:
	PlayerCardType mCardType;
};