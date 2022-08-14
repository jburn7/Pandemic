#include "Deck.h"
#include "ColorManager.h"
#include "game.h"
#include "EventSystem.h"

template<typename T>
Deck<T>::Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : mPosition(pos), mDeckName(deckName)
{
	mTextBox = createDeckNameText(doc);
}

template<typename T>
Deck<T>::~Deck()
{
	mTextBox = NULL;
}

template<typename T>
void Deck<T>::addCard(T * card, bool toBack)
{
	card->setPosition(mPosition);
	if(mCards.size() > 0)
	{
		mCards[mCards.size() - 1]->setIsHidden(true);
	}
	card->setIsHidden(false);
	if(toBack)
	{
		mCards.push_back(card);
	}
	else
	{
		mCards.insert(mCards.begin(), card);
	}
}

template<typename T>
bool Deck<T>::checkDeckForClick(Vector2D clickPosition, const std::string & opener) const
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

template<typename T>
T * Deck<T>::dealTopCard()
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

template<typename T>
void Deck<T>::shuffle()
{
	std::random_shuffle(mCards.begin(), mCards.end());
}

template<typename T>
Vector2D Deck<T>::getPosition() const
{
	return mPosition;
}

template<typename T>
UIBox* Deck<T>::createDeckNameText(const rapidjson::Document &doc)
{
	ColorManager& colorManager = *ColorManager::getInstance();
	int deckNameFontSize = doc["deck"]["nameFontSize"].GetInt();
	int deckNamePadding = doc["deck"]["namePadding"].GetInt();
	int cardSize = doc["deck"]["cardHeight"].GetInt();
	UIBox *deckNameText = new UIBox(
		Vector2D(mPosition.getX(), mPosition.getY() + deckNamePadding + cardSize),
		deckNameFontSize,
		colorManager.color(doc["ui"]["darkUIColor"].GetString()),
		mDeckName
	);
	deckNameText->setOutline(Outline(colorManager.white, colorManager.white, deckNamePadding));
	gpEventSystem->fireEvent(new UnitAddEvent(UNIT_ADD_EVENT, deckNameText));

	return deckNameText;
}

template class Deck<PlayerCard>;
template class Deck<InfectionCard>;