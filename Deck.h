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
	Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName);
	virtual ~Deck();

	void addCard(T *card);
	bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const;
	T* dealTopCard();
	void shuffle();

	// Getters
	Vector2D getPosition() const;
protected:
	Vector2D mPosition;

private:
	UIBox* createDeckNameText(const rapidjson::Document &doc);
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