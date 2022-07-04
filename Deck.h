#pragma once
#include "Vector2D.h"
#include "uiBox.h"
#include "document.h"
#include "Trackable.h"
#include "PlayerCard.h"
#include "InfectionCard.h"

class Deck : public Trackable
{
public:
	Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName);
	virtual ~Deck();

	// Getters
	Vector2D getPosition() const;

protected:
	Vector2D mPosition;

private:
	UIBox* createDeckNameText(const rapidjson::Document &doc);
	const std::string mDeckName;
	UIBox *mTextBox;
};

class PlayerCardDeck : public Deck
{
public:
	PlayerCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : Deck(doc, pos, deckName) {}
	virtual ~PlayerCardDeck() {}
	void addCard(PlayerCard *card)
	{
		card->setPosition(mPosition);
		if(mCards.size() > 0)
		{
			mCards[mCards.size() - 1]->setIsHidden(true);
		}
		card->setIsHidden(false);
		mCards.push_back(card);
	}
	virtual bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const
	{
		// Hacky, but we'll just ask the top card in each stack for its position
		if(mCards.size() > 0)
		{
			if(mCards[0]->contains(clickPosition))
			{
				std::cout << opener << std::endl;

				for(auto &a : mCards)
				{
					std::cout << a->debugDescription() << std::endl;
				}

				return true;
			}
		}

		return false;
	}
	PlayerCard* dealTopCard()
	{
		if(mCards.size > 0)
		{
			PlayerCard* pc = *mCards.begin();
			pc->setIsHidden(false);
			mCards.erase(mCards.begin());
			if(mCards.size() > 0)
			{
				mCards[0]->setIsHidden(false);
			}

			return pc;
		}
		else
		{
			return nullptr;
		}
	}
	void shuffle()
	{
		std::random_shuffle(mCards.begin(), mCards.end());
	}
private:
	std::vector<PlayerCard*> mCards;
};

class InfectionCardDeck : public Deck
{
public:
	InfectionCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : Deck(doc, pos, deckName) {}
	virtual ~InfectionCardDeck() {}
	void addCard(InfectionCard *card)
	{
		card->setPosition(mPosition);
		if(mCards.size() > 0)
		{
			mCards[mCards.size() - 1]->setIsHidden(true);
		}
		card->setIsHidden(false);
		mCards.push_back(card);
	}
	virtual bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const
	{
		// Hacky, but we'll just ask the top card in each stack for its position
		if(mCards.size() > 0)
		{
			if(mCards[0]->contains(clickPosition))
			{
				std::cout << opener << std::endl;

				for(auto &a : mCards)
				{
					std::cout << a->debugDescription() << std::endl;
				}

				return true;
			}
		}

		return false;
	}
	InfectionCard* dealTopCard()
	{
		if(mCards.size > 0)
		{
			InfectionCard* card = *mCards.begin();
			card->setIsHidden(false);
			mCards.erase(mCards.begin());
			if(mCards.size() > 0)
			{
				mCards[0]->setIsHidden(false);
			}

			return card;
		}
		else
		{
			return nullptr;
		}
	}
	void shuffle()
	{
		std::random_shuffle(mCards.begin(), mCards.end());
	}
private:
	std::vector<InfectionCard*> mCards;
};