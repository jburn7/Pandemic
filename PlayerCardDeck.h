#pragma once
#include "Deck.h"

class PlayerCardDeck : public Deck<PlayerCard>
{
public:
	PlayerCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName);
	virtual ~PlayerCardDeck();

	virtual bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const;

	// Caller must delete any epidemic cards returned
	virtual PlayerCard* dealTopCard() override;

	// Tell deck that initialization has been completed
	void completeInit();
private:
	/* This usage is a little funny because player cards can be added/removed during play, but epidemic cards can only be removed.
	   Because epidemic cards are not actually city cards, the best thing to do is have the player deck generate them JIT for a new player card. So basically, upon initialization, it will set its indeces for when it will generate a player card upon drawing if the initial deck size - current deck size == next epidemic index, then it will pop that epidemic index so it can't be triggered if cards are added later. The caller will have to be mindful of the return value for dealTopCard - it will return a card with a null city pointer, so it will be a dangerous value if the caller treats it normally.
	*/
	std::vector<int> mEpidemicIndeces;
	int mInitialDeckSize;
	unsigned int mNumEpidemics;
};