#pragma once
#include "graphicsSystem.h"
#include "unitManager.h"
#include "graphicsBufferManager.h"
#include "inputSystem.h"
#include "Timer.h"
#include "PerformanceTracker.h"
#include "jsonData.h"
#include "levelManager.h"
#include "updateFPSEvent.h"
#include "ui.h"
#include "Board.h"

enum Gamestate
{
	START,
	PLAYING,
	COMPLETE
};

//provides framework for an actual game implementation which will use this and all classes this depends on
//OR means that you probably want to override the commented function if you want any extra functionality
//most to all overridden functions will still want to call their super function so as not to miss any vital functionality

class Game : public EventListener
{
public:
	static Game *getInstance();
	static void initInstance();
	static void cleanupInstance();

	void init(const std::string &jsonPath);

	void cleanup();

	void complete();

	void loop();

	//event functions
	virtual void handleEvent(const Event& theEvent);
	
	//getters
	Board &getBoard(){ return mBoard; }
	Font &getDefaultFont(){ return mFont; }
	Gamestate &getGamestate(){ return mGamestate; }
	GraphicsBufferManager &getGraphicsBufferManager();
	GraphicsSystem &getGraphics();
	InputSystem &getInputSystem();
	//LevelManager &getLevelManager(){ return mLevelManager; }
	UnitManager &Game::getUnitManager();
protected:
	Game();
	~Game();

	void restartGame();

	Gamestate mGamestate;

	std::map<std::string, Animation*> loadUnitAnimations(rapidjson::Value &a, int tileSize);

	void processInput(); //OR
	void update(double timeElapsed);
	void render();

	GraphicsBufferManager mGraphicsBufferManager;
	InputSystem mInputSystem;
	GraphicsSystem mGraphics;
	//LevelManager mLevelManager;
	UnitManager mUnitManager;

	Board mBoard;
	Font mFont;
	UI *mUi;

	bool mQuit, mJustReset;
	Timer mClock;

	const char *JSONPATH = "game";

	static Game *msInstance;
};