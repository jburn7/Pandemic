#pragma once
#include "Vector2D.h"
#include "uiBox.h"
#include "document.h"
#include "Trackable.h"
#include "PlayerCard.h"
#include "InfectionCard.h"
#include "ColorManager.h"
#include "game.h"
#include "EventSystem.h"

template<typename T>
class Deck : public Trackable
{
public:
	Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : mPosition(pos), mDeckName(deckName)
	{
		mTextBox = createDeckNameText(doc);
	}
	virtual ~Deck()
	{
		mTextBox = NULL;
	}

	void addCard(T *card)
	{
		card->setPosition(mPosition);
		if(mCards.size() > 0)
		{
			mCards[mCards.size() - 1]->setIsHidden(true);
		}
		card->setIsHidden(false);
		mCards.push_back(card);
	}
	bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const
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
	void shuffle()
	{
		std::random_shuffle(mCards.begin(), mCards.end());
	}

	// Getters
	Vector2D getPosition() const
	{
		return mPosition;
	}

	T* dealTopCard()
	{
		if(mCards.size() > 0)
		{
			T* card = static_cast<T*>(*mCards.begin());
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

protected:
	Vector2D mPosition;

private:
	UIBox* createDeckNameText(const rapidjson::Document &doc)
	{
		ColorManager& colorManager = *ColorManager::getInstance();
		int deckNameFontSize = doc["deck"]["nameFontSize"].GetInt();
		float deckNamePadding = doc["deck"]["namePadding"].GetFloat();
		int cardSize = doc["deck"]["cardHeight"].GetInt();
		UIBox *deckNameText = new UIBox(
			Vector2D(mPosition.getX(), mPosition.getY() + deckNamePadding + cardSize),
			deckNameFontSize,
			Vector2D(0, 0),
			colorManager.color(doc["ui"]["defaultUIColor"].GetString()),
			deckNamePadding,
			mDeckName,
			new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["deck"]["nameBackgroundImg"].GetString())),
			new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer(doc["ui"]["defaultUIPaddingImage"].GetString()))
		); //deletes called in unit manager dtor
		gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, deckNameText));

		return deckNameText;
	}
	const std::string mDeckName;
	UIBox *mTextBox;
	std::vector<T*> mCards;
};

class PlayerCardDeck : public Deck<PlayerCard>
{
public:
	PlayerCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : Deck(doc, pos, deckName) {}
	virtual ~PlayerCardDeck() {}
};

class InfectionCardDeck : public Deck<InfectionCard>
{
public:
	InfectionCardDeck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : Deck(doc, pos, deckName) {}
	virtual ~InfectionCardDeck() {}
};