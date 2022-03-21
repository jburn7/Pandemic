#include "ColorManager.h"

ColorManager *ColorManager::msInstance = nullptr;

ColorManager::ColorManager(rapidjson::Value& const doc)
{
	for(auto &v : doc.GetArray())
	{
		colors[v["name"].GetString()] = Color(v["r"].GetInt(), v["g"].GetInt(), v["b"].GetInt());
	}

	black = colors["black"];
	orange = colors["orange"];
	purple = colors["purple"];
	teal = colors["teal"];
	white = colors["white"];
}

ColorManager::~ColorManager()
{
}

ColorManager *ColorManager::getInstance()
{
	return msInstance;
}

void ColorManager::init(rapidjson::Value& const doc)
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


