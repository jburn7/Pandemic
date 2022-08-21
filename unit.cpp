#include "unit.h"
#include "game.h"

Unit::Unit()
{
	mPosition.setX(0);
	mPosition.setY(0);
	mTheta = 0;
	mScale = Vector2D(1, 1);
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

Unit::Unit(Vector2D pos, Sprite *s) : Unit()
{
	mPosition = pos;
	mConstantFrame = s;
	mAnimating = false;
}

Unit::Unit(const Vector2D &pos, int width, int height) : Unit()
{
	mPosition = pos;
	mConstantFrame = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("default"));
	setScale((float)width, (float)height);
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

void Unit::draw()
{
	if(mAnimation)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mAnimation->getCurrentSprite(), mTheta, mScale, mOutline);
	}
	else if(mConstantFrame)
	{
		Game::getInstance()->getGraphics().draw(mPosition, *mConstantFrame, mTheta, mScale, mOutline);
	}
}

void Unit::update(double timeElapsed)
{
	if(mAnimating && mAnimation)
	{
		mAnimation->update(timeElapsed);
	}
}

void Unit::move(const Vector2D &delta)
{
	setPosition(mPosition + delta);
}

bool Unit::contains(Vector2D &loc)
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

void Unit::setPosition(Vector2D pos)
{
	mPosition = pos;
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

Vector2D &Unit::getPosition()
{
	return mPosition;
}

bool Unit::getIsHidden()
{
	return mIsHidden;
}

bool Unit::getIsGuiLayer()
{
	return mIsGuiLayer;
}

int Unit::getWidth()
{
	if(mAnimation)
	{
		return mAnimation->getCurrentSprite()->getWidth() * (int)mScale.getX();
	}
	else if(mConstantFrame)
	{
		return mConstantFrame->getWidth() * (int)mScale.getX();
	}

	return (int)mScale.getX();
}

int Unit::getHeight()
{
	if(mAnimation)
	{
		return mAnimation->getCurrentSprite()->getHeight() * (int)mScale.getY();
	}
	else if(mConstantFrame)
	{
		return mConstantFrame->getHeight() * (int)mScale.getY();
	}

	return (int)mScale.getY();
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

void Unit::collide(Unit * o)
{
	o->collide(this);
}

bool Unit::squareCollision(Unit *other)
{
	return (mPosition.getX() < other->getPosition().getX() + other->getWidth() && mPosition.getX() + getWidth() > other->getPosition().getX() && mPosition.getY() < other->getPosition().getY() + other->getHeight() && mPosition.getY() + getHeight() > other->getPosition().getY());
}