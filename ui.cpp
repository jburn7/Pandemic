#include "ui.h"
#include "game.h"

const int DEF_UI_FONT_SIZE = 20;

UI::UI()
{
	mPos.setX(200);
	mPos.setY(200);
	mFps = 0;
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	int r, g, b;

	rapidjson::Value &ui = doc["ui"];
	r = ui["rColor"].GetInt();
	g = ui["gColor"].GetInt();
	b = ui["bColor"].GetInt();
	mUIColor = Color(r, g, b);

	mStartString = ui["startString"].GetString();
	mFailedString = ui["failedString"].GetString();
	mCompleteString = ui["completeString"].GetString();
	mReplayString = ui["replayString"].GetString();

	gpEventSystem->addListener(UPDATE_FPS_EVENT, this);
}

UI::~UI()
{
}

void UI::handleEvent(const Event& theEvent)
{
	//if(theEvent.getType() == INCREMENT_LIVES_EVENT)
	//{
	//	const IncrementLivesEvent &ev = static_cast<const IncrementLivesEvent&>(theEvent);
	//	mLives += ev.getIncr();
	//}
	//else if(theEvent.getType() == INCREMENT_SCORE_EVENT)
	//{
	//	const IncrementScoreEvent &ev = static_cast<const IncrementScoreEvent&>(theEvent);
	//	mScore += ev.getScore();
	//}
	if(theEvent.getType() == UPDATE_FPS_EVENT)
	{
		const UpdateFPSEvent &ev = static_cast<const UpdateFPSEvent&>(theEvent);
		mFps = ev.getFPS();
	}
}

void UI::draw()
{
	GraphicsSystem &graphics = Game::getInstance()->getGraphics();
	int width = graphics.getWidth();
	int height = graphics.getHeight();
	graphics.writeText(Vector2D(0, 0), DEF_UI_FONT_SIZE, *mFont, mUIColor, "FPS: " + std::to_string(mFps));
	graphics.writeText(Vector2D(0, (float)DEF_UI_FONT_SIZE), DEF_UI_FONT_SIZE, *mFont, mUIColor, "Moves remaining: " + std::to_string(Game::getInstance()->getBoard().getMovesRemaining()));
	
	Gamestate &gamestate = Game::getInstance()->getGamestate();
	switch(gamestate)
	{
	case START:
		//need to center this
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mStartString, DEF_UI_FONT_SIZE) / 2, (float)height / 2), DEF_UI_FONT_SIZE, *mFont, mUIColor, mStartString);
		break;
	//case DEATH:
	//	graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mDeathString) / 2, height / 2), *mFont, mUIColor, mDeathString);
	//	break;
	//case FAILED:
	//	graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mFailedString) / 2, height / 2), *mFont, mUIColor, mFailedString);
	//	graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayString) / 2, height / 2 + mFont->getFontSize()), *mFont, mUIColor, mReplayString);
	//	graphics.writeText(Vector2D(width / 2 - mFont->getWidth(mReplayStringLine2) / 2, height / 2 + 2 * mFont->getFontSize()), *mFont, mUIColor, mReplayStringLine2);
	//	break;
	case COMPLETE:
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mCompleteString, DEF_UI_FONT_SIZE) / 2, (float)height / 2), DEF_UI_FONT_SIZE, *mFont, mUIColor, mCompleteString);
		graphics.writeText(Vector2D((float)width / 2 - mFont->getWidth(mReplayString, DEF_UI_FONT_SIZE) / 2, (float)height / 2 + DEF_UI_FONT_SIZE), DEF_UI_FONT_SIZE, *mFont, mUIColor, mReplayString);
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

void UI::setFont(Font *font)
{
	mFont = font;
}
