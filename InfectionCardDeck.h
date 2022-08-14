#pragma once
#include "Deck.h"

class InfectionCardDeck : public Deck<InfectionCard>
{
public:
	InfectionCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName);
	virtual ~InfectionCardDeck();

	InfectionCard* drawLastCard();
	void moveContentsToDeck(InfectionCardDeck* destination);
};