#pragma once
#include "Trackable.h"
#include "document.h"
#include "color.h"
#include <unordered_map>

class ColorManager : public Trackable
{
public:
	static ColorManager *getInstance();
	static void init(rapidjson::Value& const doc);
	static void cleanup();

	// To add a new color, add color to colors in json, add its name here, and assign it by name in ctor.
	// TODO: make const and/or get-only
	Color 
		black,
		orange,
		purple,
		teal,
		white;

	std::unordered_map<std::string, Color> colors;
	
protected:
	ColorManager(rapidjson::Value& const doc);
	~ColorManager();

	static ColorManager* msInstance;
};