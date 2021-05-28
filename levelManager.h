#pragma once
#include "jsonData.h"
#include "level.h"
#include "EventListener.h"
#include <vector>
#include "Trackable.h"

class LevelManager : public EventListener
{
public:
	LevelManager();
	~LevelManager();

	void init();
	void cleanup();

	void update(double timeElapsed);

	virtual void handleEvent(const Event& theEvent);

	//getters
	Level &getCurrentLevel(){ return mLevels[mCurrentLevel]; }

	bool loadNewLevel();
	void loadLevel();
protected:
	//must be hardcoded so that all other data isn't hardcoded
	const char *JSONPATH = "level";

	int mCurrentLevel;
	int mMaxLevels;
	std::string mLevelsPath;

	std::vector<Level> mLevels;
};