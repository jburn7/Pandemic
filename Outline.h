#pragma once
#include "color.h"

struct Outline
{
	Outline(const Color &color = Color(), int thickness = 0) : color(color), thickness(thickness) {}
	Color color;
	int thickness;
};