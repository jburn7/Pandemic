#include "ui.h"
#include "game.h"
#include "PawnChangeEvents.h"

UI::UI()
{
	mFps = 0;
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();

	mFontSize = doc["ui"]["defaultFontSize"].GetInt();
	mPlayerDetailsXOffset = doc["ui"]["playerDetailsXOffset"].GetInt();

	int r, g, b;

	const rapidjson::Value &ui = doc["ui"];
	r = ui["rColor"].GetInt();
	g = ui["gColor"].GetInt();
	b = ui["bColor"].GetInt();
	mUIColor = Color(r, g, b);

	mStartString = ui["startString"].GetString();
	mFailedString = ui["failedString"].GetString();
	mCompleteString = ui["completeString"].GetString();
	mReplayString = ui["replayString"].GetString();

	mActivePawnSprite = nullptr;

	gpEventSystem->addListener(EventType::UPDATE_FPS_EVENT, this);
	gpEventSystem->addListener(EventType::ACTIVE_PAWN_CHANGE_EVENT, this);
}

UI::~UI()
{
}

void UI::handleEvent(const Event& theEvent)
{
	if(theEvent.getType() == EventType::UPDATE_FPS_EVENT)
	{
		const UpdateFPSEvent &ev = static_cast<const UpdateFPSEvent&>(theEvent);
		mFps = ev.getFPS();
	}
	else if(theEvent.getType() == EventType::ACTIVE_PAWN_CHANGE_EVENT)
	{
		const ActivePawnChangeEvent &ev = static_cast<const ActivePawnChangeEvent&>(theEvent);
		mActivePawnSprite = ev.getPawn().getSprite();
	}
}

void UI::draw()
{
	GraphicsSystem &graphics = Game::getInstance()->getGraphics();
	int width = graphics.getWidth();
	int height = graphics.getHeight();
	const Vector2D screenTopLeft = Vector2D(0, 0);

	graphics.writeText(screenTopLeft + Vector2D(0, 0), mFontSize, *mFont, mUIColor, "FPS: " + std::to_string(mFps));
	graphics.writeText(screenTopLeft + Vector2D(0, (float)mFontSize * 1), mFontSize, *mFont, mUIColor, "Moves remaining: " + std::to_string(Game::getInstance()->getBoard().getMovesRemaining()));
	graphics.writeText(screenTopLeft + Vector2D(0, (float)mFontSize * 2), mFontSize, *mFont, mUIColor, "Epidemics had: " + std::to_string(Game::getInstance()->getBoard().getNumEpidemicsHad()));

	const std::string activePawnString = std::string("Active pawn: ");
	graphics.writeText(screenTopLeft + Vector2D(0, (float)mFontSize * 3), mFontSize, *mFont, mUIColor, activePawnString);

	graphics.writeText(screenTopLeft + Vector2D((float)mPlayerDetailsXOffset, (float)mFontSize * 2), mFontSize, *mFont, mUIColor, "Player Details: ");

	const int activePawnStringWidth = mFont->getWidth(activePawnString, mFontSize);
	const int spriteHeight = mActivePawnSprite->getHeight();
	const int desiredSpriteHeight = mFontSize;
	const float spriteHeightRatio = (float)desiredSpriteHeight / spriteHeight;
	const float originAdjust = mFont->getUnderlineSpacing(mFontSize);
	graphics.draw(screenTopLeft + Vector2D((float)activePawnStringWidth, (float)mFontSize * 3 + originAdjust), *mActivePawnSprite, 0, Vector2D(spriteHeightRatio, spriteHeightRatio));
	
	Gamestate &gamestate = Game::getInstance()->getGamestate();
	switch(gamestate)
	{
	case Gamestate::START:
		//need to center this
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mStartString, mFontSize) / 2, (float)height / 2), mFontSize, *mFont, mUIColor, mStartString);
		break;
	case Gamestate::COMPLETE:
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mCompleteString, mFontSize) / 2, (float)height / 2), mFontSize, *mFont, mUIColor, mCompleteString);
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mReplayString, mFontSize) / 2, (float)height / 2 + mFontSize), mFontSize, *mFont, mUIColor, mReplayString);
		break;
	}
}

void UI::update()
{
	//Gamestate &gamestate = Game::getInstance()->getGamestate();
	//switch(gamestate)
	//{
	//case START:
	//	break;
	//case PLAYING:
	//	break;
	//case COMPLETE:
	//	break;
	//}
}

void UI::reset()
{
	mFps = 0;
}

int UI::getFontSize()
{
	return mFontSize;
}

void UI::setFont(Font *font)
{
	mFont = font;
}
