#include "ColorManager.h"

ColorManager *ColorManager::msInstance = nullptr;

ColorManager::ColorManager(rapidjson::Value const& doc)
{
	for(auto &v : doc.GetArray())
	{
		mColors[v["name"].GetString()] = Color(v["r"].GetInt(), v["g"].GetInt(), v["b"].GetInt(), v["a"].GetInt());
	}

	mBlack = mColors["black"];
	mClear = mColors["clear"];
	mOrange = mColors["orange"];
	mPurple = mColors["purple"];
	mTeal = mColors["teal"];
	mWhite = mColors["white"];
}

ColorManager::~ColorManager()
{
}

ColorManager *ColorManager::getInstance()
{
	return msInstance;
}

void ColorManager::init(rapidjson::Value const& doc)
{
	msInstance = new ColorManager(doc);
}

void ColorManager::cleanup()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = nullptr;
	}
}

Color ColorManager::color(const std::string &name)
{
	if(mColors.find(name) == mColors.end())
	{
		return Color();
	}

	return mColors[name];
}


