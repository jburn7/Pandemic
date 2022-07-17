#pragma once
#include "color.h"

struct Outline
{
	Outline(const Color &borderColor = Color(), const Color &fillColor = Color(0, 0, 0, 0), int thickness = 0) : borderColor(borderColor), fillColor(fillColor), thickness(thickness) {}
	Color borderColor;
	Color fillColor;
	int thickness;
};