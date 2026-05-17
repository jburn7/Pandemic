#pragma once
#include "graphicsBuffer.h"
#include "Vector2D.h"

class Unit;

class Sprite : public Trackable
{
public:
	Sprite();

	Sprite(const GraphicsBuffer &g);

	~Sprite();

	//setters
	void setColor(const Color &c);
	void setRotation(double theta);
	void setOrigin(Vector2D o);
	void setSourceLoc(int x, int y);
	void setTexture(GraphicsBuffer *g);
	void setUnit(const Unit* const unit);

	//getters
	Color getColor() const;
	double getRotation() const;
	Vector2D getOrigin() const;
	int getTransparency() const;

	Vector2D getSourceLoc() const;

	const GraphicsBuffer *getTexture() const;
protected:
	Vector2D mSourceLoc;
	Vector2D mOrigin; //used to rotate about a specific point/accomodate for certain graphics libraries that are pedantic about rotations cough SFML cough
	double mTheta;
	Color mColor;
	int mAlpha;

	const GraphicsBuffer* mTexture;
	Unit const* mpUnit;
};