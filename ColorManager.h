#pragma once
#include "Trackable.h"
#include "document.h"
#include "color.h"
#include <unordered_map>

class ColorManager : public Trackable
{
public:
	static ColorManager *getInstance();
	static void init(rapidjson::Value const& doc);
	static void cleanup();

	// In case color was not manually added to list of get-only colors
	Color color(const std::string &name);

	// Get-only color names
	const Color
		&black = mBlack,
		&clear = mClear,
		&orange = mOrange,
		&purple = mPurple,
		&teal = mTeal,
		&white = mWhite;

protected:
	ColorManager(rapidjson::Value const& doc);
	~ColorManager();

	static ColorManager* msInstance;

	// To add a new color, add color to colors in json
	// For convenience, you can also add its name here, and assign it by name in ctor.
	Color
		mBlack,
		mClear,
		mOrange,
		mPurple,
		mTeal,
		mWhite;

	std::unordered_map<std::string, Color> mColors;
};