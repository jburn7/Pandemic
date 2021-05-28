#pragma once
#include "graphicsBuffer.h"
#include "Vector2D.h"

class Sprite : public Trackable
{
public:
	Sprite();

	Sprite(GraphicsBuffer &g);

	~Sprite();

	//setters
	void setHeight(int h);
	void setRotation(double theta);
	void setOrigin(Vector2D o);
	void setWidth(int w);
	void setSourceLoc(int x, int y);
	void setTexture(GraphicsBuffer *g);

	//getters
	Color *getColor();
	int getHeight();
	double getRotation();
	Vector2D getOrigin();
	int getTransparency();
	int getWidth();

	Vector2D getSourceLoc();

	GraphicsBuffer *getTexture();
protected:
	Vector2D mSourceLoc;
	Vector2D mOrigin; //used to rotate about a specific point/accomodate for certain graphics libraries that are pedantic about rotations cough SFML cough
	int mWidth, mHeight;
	double mTheta;
	Color mColor;
	int mAlpha;

	GraphicsBuffer *mTexture;
};