#include "EpidemicCard.h"

EpidemicCard::EpidemicCard(Vector2D pos, const GraphicsBuffer& graphicsBuffer) : PlayerCard(pos, graphicsBuffer, nullptr)
{
	mCardType = PlayerCardType::EPIDEMIC;
}
