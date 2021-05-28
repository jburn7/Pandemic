#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "unit.h"
#include "EventSystem.h"
#include "unitAddEvent.h"
#include "sprite.h"
#include "jsonData.h"

class Level
{
	friend class LevelManager;
public:
	Level(std::string &filename, int levelNumber);
	~Level();

	void update(double timeElapsed);

	std::string &getLevelString(){ return mLevelString; }
protected:
	void load();

	int mTileSize; //width/height of walls
	int mLevelNumber;
	std::string mFilename;

	std::string mLevelString;

	std::vector<Vector2D> mItemSpawns;
};