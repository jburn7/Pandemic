 #pragma once
#include "animation.h"
#include "Moveable.h"
#include "Shape.h"
#include "sprite.h"
#include "Outline.h"

constexpr double PI = 3.1415926535897932;
constexpr int MAX_Z_LAYERS = 4;

//declare any classes that require double dispatch for collision here

// TODO: I think we can keep public Trackable here and instead use compositon for Moveable
// And then in the overall MovementManager collection, we can still store a hash of Moveables and just pass the reference to both the Unit and CameraManager's moveables (or make them pointers)
class Unit : public Moveable
{
	friend class UnitManager;
	friend class GraphicsSystem;
	friend class UI;
public:
	Unit(Vector2D pos, Animation *a);

	Unit(Vector2D pos, const GraphicsBuffer& graphicsBuffer);

	Unit(Vector2D pos, Animation* a, const Shape& shape);

	Unit(Vector2D pos, const GraphicsBuffer& graphicsBuffer, const Shape& shape);

	Unit(const Vector2D &pos, const int width, const int height);

	virtual ~Unit();

	void cleanup();

	virtual void draw() const;

	virtual void update(double timeElapsed);

	virtual std::string debugDescription() { return ""; }

	//collision functions
	bool contains(Vector2D &loc) const;

	//setters
	void setAnimating(bool torf);
	void setColor(const Color &color);
	void setIsHidden(const bool isHidden);
	void setIsGuiLayer(const bool isGui);
	void setSprite(Sprite *s);
	void setRotation(double theta);
	void adjustScale(double delta);
	void setScale(float x, float y);
	void setOutline(const Outline &outline);
	void setZLayer(int z);

	void clearOutline();

	//getters
	Vector2D getCenter();
	bool getIsHidden();
	bool getIsGuiLayer();
	virtual int getWidth() const;
	virtual int getHeight() const;
	int getZLayer();

	//collision functions, using double dispatch because I despise static casting
	virtual void collide(Unit *o);
	//virtual void collide(ScorePowerUp *o) {}

	bool squareCollision(Unit *other);
protected:
	Unit();
	virtual bool tryRemove(){ return true; } //returns true if the object is removable such as a powerup, false if it's something like a wall

	// We want to abstract the sprite as much as possible, but there are some cases where a direct sprite reference is needed (eg UI pawn info)
	const Sprite *getSprite() const;
	const Shape& getShape() const;

	bool mAnimating;
	double mTheta; //for storing rotation data

	int mZLayer; // UnitManager.draw reads this and decides how to position units on top of one another. 0 is farthest "back" and all units default to most front layer
	bool mIsHidden;
	bool mIsGuiLayer; // Probably a better way to do this but I need to draw some items onto a separate "gui" view so they don't scale with the camera. Ideally I would have UnitManeger store them separately instead of iterating through units twice, but we can cross that bridge if it's too slow

	Vector2D mScale;
	
	Animation* mAnimation;
	Color mColor;
	Outline mOutline;
	Shape mShape;
	Sprite *mConstantFrame; //either sprite* or animation* will == NULL, depending on whether the Unit has an animation sequence or not
};