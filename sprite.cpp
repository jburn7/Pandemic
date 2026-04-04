#include "sprite.h"

Sprite::Sprite() : mOrigin(0, 0), mColor(255, 255, 255)
{
	mTheta = 0;
	mAlpha = 255;
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
	mTexture = NULL;
}

Sprite::Sprite(const GraphicsBuffer &g) : mOrigin(0, 0), mColor(255, 255, 255)
{
	mTexture = &g;
	mTheta = 0;
	mAlpha = 255;
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
}

Sprite::~Sprite()
{
}

void Sprite::setColor(const Color &c)
{
	mColor = c;
}

void Sprite::setRotation(double theta)
{
	mTheta = theta;
}

void Sprite::setOrigin(Vector2D o)
{
	mOrigin = o;
}

void Sprite::setSourceLoc(int x, int y)
{
	mSourceLoc.setX((float)x);
	mSourceLoc.setY((float)y);
}

void Sprite::setTexture(GraphicsBuffer *g)
{
	mTexture = g;
}

void Sprite::setUnit(const Unit* const unit)
{
	mpUnit = unit;
}

Color Sprite::getColor() const
{
	return mColor;
}

double Sprite::getRotation() const
{
	return mTheta;
}

Vector2D Sprite::getOrigin() const
{
	return mOrigin;
}

int Sprite::getTransparency() const
{
	return mAlpha;
}

Vector2D Sprite::getSourceLoc() const
{
	return mSourceLoc;
}

const GraphicsBuffer *Sprite::getTexture() const
{
	return mTexture;
}