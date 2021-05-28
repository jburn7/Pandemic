#include "level.h"
#include "game.h"

Level::Level(std::string &filename, int levelNumber)
{
	mLevelNumber = levelNumber;
	mFilename = filename;

	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	std::string jsonLevelPath = "level" + std::to_string(mLevelNumber + 1);
}

Level::~Level()
{
}

void Level::update(double timeElapsed)
{
}

void Level::load()
{
	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	mTileSize = doc["game"]["tileSize"].GetInt();

	mLevelString = "level" + std::to_string(mLevelNumber + 1);

	std::ifstream fin(mFilename);
	std::string line;
	int y = 0;
	while(std::getline(fin, line))
	{
		for(unsigned int i = 0; i < line.length(); i++)
		{
			//read text file and populate level with units corresponding to characters
		}
		y++;
	}

	fin.close();
}