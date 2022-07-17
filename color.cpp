#include "Color.h"
#include "graphicsBuffer.h"

Color::Color(int red, int green, int blue, int alpha)
{
	if(red < 0 || red > 255)
	{
		red = 0;
	}
	if(green < 0 || green > 255)
	{
		green = 0;
	}
	if(blue < 0 || blue > 255)
	{
		blue = 0;
	}
	if(alpha < 0 || alpha > 255)
	{
		alpha = 0;
	}

	mColor.r = red;
	mColor.b = blue;
	mColor.g = green;
	mColor.a = alpha;
}

Color::~Color()
{
}