#include "levelManager.h"
#include "game.h"

LevelManager::LevelManager()
{
	mCurrentLevel = 0;
	mMaxLevels = 0;

	//register as event listener if needed
}

LevelManager::~LevelManager()
{
}

void LevelManager::init()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mMaxLevels = doc[JSONPATH]["maxLevels"].GetInt();
	mCurrentLevel = doc[JSONPATH]["startingLevel"].GetInt() - 1; //will be incremented in loadnewLevel()
	mLevelsPath = doc[JSONPATH]["levelsPath"].GetString();

	for(int i = 0; i < mMaxLevels; i++)
	{
		mLevels.push_back(Level(mLevelsPath + std::to_string(i + 1) + ".txt", i));
	}

	mLevels[mCurrentLevel].load();
}

void LevelManager::cleanup()
{
}

void LevelManager::update(double timeElapsed)
{
	mLevels[mCurrentLevel].update(timeElapsed);
}

void LevelManager::handleEvent(const Event &theEvent)
{
}

bool LevelManager::loadNewLevel()
{
	if(mCurrentLevel + 1 < mMaxLevels)
	{
		mCurrentLevel++;
		Game::getInstance()->getUnitManager().setLevelEnded(true);
		return true;
	}
	else
	{
		Game::getInstance()->complete();
	}

	return false;
}

void LevelManager::loadLevel()
{
	mLevels[mCurrentLevel].load();
}
