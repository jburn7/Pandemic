#include "InfectionCardDeck.h"

InfectionCardDeck::InfectionCardDeck(const rapidjson::Document & doc, const Vector2D pos, const std::string & deckName) : Deck(doc, pos, deckName)
{
}

InfectionCardDeck::~InfectionCardDeck()
{
}

InfectionCard *InfectionCardDeck::drawLastCard()
{
	if(mCards.size() > 0)
	{
		InfectionCard* card = mCards.back();
		mCards.pop_back();
		return card;
	}
	else
	{
		return nullptr;
	}
}

void InfectionCardDeck::moveContentsToDeck(InfectionCardDeck *destination)
{
	for(auto &card : mCards)
	{
		destination->addCard(card, false);
	}
	mCards.clear();
}
