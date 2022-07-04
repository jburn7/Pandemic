#include "Deck.h"
#include "ColorManager.h"
#include "game.h"
#include "EventSystem.h"

Deck::Deck(const rapidjson::Document &doc, const Vector2D pos, const std::string &deckName) : mPosition(pos), mDeckName(deckName)
{
	mTextBox = createDeckNameText(doc);
}

Deck::~Deck()
{
	mTextBox = NULL;
}

Vector2D Deck::getPosition() const
{
	return mPosition;
}

UIBox* Deck::createDeckNameText(const rapidjson::Document &doc)
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
