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
	void setColor(const Color &c);
	void setRotation(double theta);
	void setOrigin(Vector2D o);
	void setWidth(int w);
	void setSourceLoc(int x, int y);
	void setTexture(GraphicsBuffer *g);

	//getters
	Color getColor() const;
	int getHeight() const;
	double getRotation() const;
	Vector2D getOrigin() const;
	int getTransparency() const;
	int getWidth() const;

	Vector2D getSourceLoc() const;

	GraphicsBuffer *getTexture() const;
protected:
	Vector2D mSourceLoc;
	Vector2D mOrigin; //used to rotate about a specific point/accomodate for certain graphics libraries that are pedantic about rotations cough SFML cough
	int mWidth, mHeight;
	double mTheta;
	Color mColor;
	int mAlpha;

	// TODO: I think sprites will want to store an optional list of vertices for outlines, or maybe this belongs in Outline itself?

	GraphicsBuffer *mTexture;
};