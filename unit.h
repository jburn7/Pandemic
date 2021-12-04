#pragma once
#include "animation.h"

#define PI 3.1415926535897932
#define MAX_Z_LAYERS 2

//declare any classes that require double dispatch for collision here

class Unit : public Trackable
{
	friend class UnitManager;
public:
	Unit();

	Unit(Vector2D pos, Animation *a);

	Unit(Vector2D pos, Sprite *s);

	virtual ~Unit();

	void cleanup();

	virtual void draw();

	virtual void update(double timeElapsed);

	//collision functions
	bool contains(Vector2D &loc);

	//setters
	void setAnimating(bool torf);
	void setColor(const Color &color);
	void setPosition(Vector2D pos);
	void setSprite(Sprite *s);
	void setRotation(double theta);
	void setScale(float x, float y);
	void setZLayer(int z);

	//getters
	Vector2D getCenter();
	Vector2D &getPosition();
	int getWidth();
	int getHeight();
	int getZLayer();

	//collision functions, using double dispatch because I despise static casting
	virtual void collide(Unit *o);
	//virtual void collide(ScorePowerUp *o) {}

	bool squareCollision(Unit *other);
protected:
	virtual bool tryRemove(){ return true; } //returns true if the object is removable such as a powerup, false if it's something like a wall

	bool mAnimating;
	double mTheta; //for storing rotation data

	int mZLayer; // UnitManager.draw reads this and decides how to position units on top of one another. 0 is farthest "back" and all units default to most front layer

	Color mColor;
	Vector2D mScale;
	Vector2D mPosition;
	Animation *mAnimation;
	Sprite *mConstantFrame; //either sprite* or animation* will == NULL, depending on whether the Unit has an animation sequence or not
};