#pragma once
#include "Vector2D.h"
#include "textBox.h"
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
	virtual ~Deck();

	void addCard(T *card, bool toBack = true);
	virtual bool checkDeckForClick(Vector2D clickPosition, const std::string &opener) const;
	virtual T* dealTopCard();
	void shuffle();

	// Getters
	Vector2D getPosition() const;
protected:
	Vector2D mPosition;

private:
	friend PlayerCardDeck;
	friend InfectionCardDeck;
	Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName);
	TextBox* createDeckNameText(const rapidjson::Document &doc);
	const std::string mDeckName;
	TextBox *mpTextBox;
	std::vector<T*> mCards;
};