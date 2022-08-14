#include "PlayerCardDeck.h"
#include "EpidemicCard.h"

PlayerCardDeck::PlayerCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : Deck(doc, pos, deckName)
{
	mEpidemicIndeces = std::vector<int>();
	mNumEpidemics = doc["game"]["numEpidemics"].GetUint();
}

PlayerCardDeck::~PlayerCardDeck()
{
}

bool PlayerCardDeck::checkDeckForClick(Vector2D clickPosition, const std::string & opener) const
{
	// Hacky, but we'll just ask the top card in each stack for its position
	if(mCards.size() > 0)
	{
		if(mCards[0]->contains(clickPosition))
		{
			std::cout << opener << std::endl;

			int numEpidemicsCounted = 0;
			for(unsigned int i = 0; i < mCards.size(); i++)
			{
				if(mEpidemicIndeces.size() > numEpidemicsCounted && (mInitialDeckSize - mCards.size() + i) == mEpidemicIndeces[numEpidemicsCounted])
				{
					std::cout << "Epidemic!" << std:: endl;
					numEpidemicsCounted++;
				}
				std::cout << mCards[i]->debugDescription() << std::endl;
			}

			return true;
		}
	}

	return false;
}

PlayerCard *PlayerCardDeck::dealTopCard()
{
	if(mEpidemicIndeces.size() > 0 && mInitialDeckSize - mCards.size() >= mEpidemicIndeces.front())
	{
		mEpidemicIndeces.erase(mEpidemicIndeces.begin());
		// TODO: epidemic card background
		return new EpidemicCard(Vector2D(0, 0), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("city_card.png")));
	}
	else
	{
		return Deck::dealTopCard();
	}
}

void PlayerCardDeck::completeInit()
{
	// "Place" epidemic cards at intervals with an average of, but no less than, the number of player cards / number of epidemics
	mInitialDeckSize = mCards.size();
	int averageEpidemicInterval = mInitialDeckSize / mNumEpidemics;
	for(unsigned int i = 0; i < mNumEpidemics; i++)
	{
		int currentEpidemicInterval = std::rand() % averageEpidemicInterval + (averageEpidemicInterval * i);
		mEpidemicIndeces.push_back(currentEpidemicInterval);
	}
}
