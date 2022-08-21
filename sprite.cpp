#include "sprite.h"

Sprite::Sprite() : mOrigin(0, 0), mColor(255, 255, 255)
{
	mWidth = 0;
	mHeight = 0;
	mTheta = 0;
	mAlpha = 255;
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
	mTexture = NULL;
}

Sprite::Sprite(GraphicsBuffer &g) : mOrigin(0, 0), mColor(255, 255, 255)
{
	mTexture = &g;
	mWidth = g.getWidth();
	mTheta = 0;
	mAlpha = 255;
	mHeight = g.getHeight();
	mSourceLoc.setX(0);
	mSourceLoc.setY(0);
}

Sprite::~Sprite()
{
}

void Sprite::setHeight(int h)
{
	mHeight = h;
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

void Sprite::setWidth(int w)
{
	mWidth = w;
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

Color Sprite::getColor() const
{
	return mColor;
}

int Sprite::getHeight() const
{
	return mHeight;
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

int Sprite::getWidth() const
{
	return mWidth;
}

Vector2D Sprite::getSourceLoc() const
{
	return mSourceLoc;
}

GraphicsBuffer *Sprite::getTexture() const
{
	return mTexture;
}