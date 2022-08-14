#include "EpidemicCard.h"

EpidemicCard::EpidemicCard(Vector2D pos, Sprite *s) : PlayerCard(pos, s, nullptr)
{
	mCardType = EPIDEMIC_CARD;
}
