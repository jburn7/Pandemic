#include "unit.h"
#include "game.h"

Unit::Unit()
{
	mPosition.setX(0);
	mPosition.setY(0);
	mTheta = 0;
	mScale = Vector2D(1, 1);
	mShape = Shape();
	mAnimation = NULL;
	mConstantFrame = NULL;
	mAnimating = false;
	mZLayer = 1;
	mIsHidden = false;
	mIsGuiLayer = false;
}

Unit::Unit(Vector2D pos, Animation *a) : Unit()
{
	mPosition = pos;
	mAnimation = a;
	mAnimating = true;
}

Unit::Unit(Vector2D pos, const GraphicsBuffer& graphicsBuffer) : Unit()
{
	mPosition = pos;
	mConstantFrame = new Sprite(graphicsBuffer);
	mAnimating = false;
	mShape = Shape(graphicsBuffer.getWidth(), graphicsBuffer.getHeight());
}

Unit::Unit(Vector2D pos, Animation* a, const Shape& shape) : Unit(pos, a)
{
	mShape = shape;
}

Unit::Unit(Vector2D pos, const GraphicsBuffer& graphicsBuffer, const Shape& shape) : Unit(pos, graphicsBuffer)
{
	mShape = shape;
}

Unit::Unit(const Vector2D &pos, int width, int height) : Unit()
{
	mPosition = pos;
	mShape = Shape(width, height);
	mConstantFrame = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("default"));
}

Unit::~Unit()
{
	cleanup();
}

void Unit::cleanup()
{
	if(mAnimation)
	{
		delete mAnimation;
		mAnimation = NULL;
	}
	if(mConstantFrame)
	{
		delete mConstantFrame;
		mConstantFrame = NULL;
	}
}

void Unit::draw() const
{
	if(mAnimation)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mAnimation->getCurrentSprite(), mShape, mTheta, mScale, mOutline);
	}
	else if(mConstantFrame)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mConstantFrame, mShape, mTheta, mScale, mOutline);
	}
}

void Unit::update(double timeElapsed)
{
	if(mAnimating && mAnimation)
	{
		mAnimation->update(timeElapsed);
	}
}

bool Unit::contains(Vector2D &loc) const
{
	return mPosition.getX() < loc.getX() && mPosition.getX() + getWidth() > loc.getX() && mPosition.getY() < loc.getY() && mPosition.getY() + getHeight() > loc.getY();
}

void Unit::setAnimating(bool torf)
{
	mAnimating = torf;
}

void Unit::setColor(const Color &color)
{
	if(mConstantFrame)
	{
		mConstantFrame->setColor(color);
	}
	else
	{
		mAnimation->setColors(color);
	}
}

void Unit::setIsHidden(const bool isHidden)
{
	mIsHidden = isHidden;
}

void Unit::setIsGuiLayer(const bool isGui)
{
	mIsGuiLayer = isGui;
}

void Unit::setSprite(Sprite *s)
{
	if(mConstantFrame)
	{
		delete mConstantFrame;
	}
	mConstantFrame = s;
}

void Unit::setRotation(double theta)
{
	mTheta = theta;
	//if unit doesn't animate then we can just set the sprite's rotation directly
	if(mConstantFrame)
	{
		mConstantFrame->setRotation(theta);
	}
}

void Unit::adjustScale(double delta)
{
	setScale(mScale.getX() + (float)delta, mScale.getY() + (float)delta);
}

void Unit::setScale(float x, float y)
{
	mScale = Vector2D(x, y);
}

void Unit::setOutline(const Outline &outline)
{
	mOutline = outline;
}

void Unit::setZLayer(int z)
{
	mZLayer = z;
}

void Unit::clearOutline()
{
	mOutline = Outline();
}

Vector2D Unit::getCenter()
{
	return Vector2D(mPosition.getX() + getWidth() / 2, mPosition.getY() + getHeight() / 2);
}

bool Unit::getIsHidden()
{
	return mIsHidden;
}

bool Unit::getIsGuiLayer()
{
	return mIsGuiLayer;
}

int Unit::getWidth() const
{
	return mShape.getWidth() * (int)mScale.getX();
}

int Unit::getHeight() const
{
	return mShape.getHeight() * (int)mScale.getY();
}

int Unit::getZLayer()
{
	return mZLayer;
}

const Sprite *Unit::getSprite() const
{
	if(mAnimation)
	{
		return mAnimation->getCurrentSprite();
	}
	else if(mConstantFrame)
	{
		return mConstantFrame;
	}

	return nullptr;
}

const Shape& Unit::getShape() const
{
	return mShape;
}

void Unit::collide(Unit * o)
{
	o->collide(this);
}

bool Unit::squareCollision(Unit *other)
{
	return (mPosition.getX() < other->getPosition().getX() + other->getWidth() && mPosition.getX() + getWidth() > other->getPosition().getX() && mPosition.getY() < other->getPosition().getY() + other->getHeight() && mPosition.getY() + getHeight() > other->getPosition().getY());
}