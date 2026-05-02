#include "Moveable.h"

Moveable::Moveable()
{
	mPosition = Vector2D();
}

Moveable::Moveable(const Vector2D position)
{
	mPosition = position;
}

void Moveable::move(const Vector2D& delta)
{
	setPosition(mPosition + delta);
}

void Moveable::setPosition(Vector2D pos)
{
	mPosition = pos;
}

const Vector2D& Moveable::getPosition() const
{
	return mPosition;
}