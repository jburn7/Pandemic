#pragma once
#include "Vector2D.h"

class Moveable : public Trackable
{
public:
	Moveable();
	Moveable(const Vector2D position);

	virtual void move(const Vector2D& delta);

	// Setters
	virtual void setPosition(Vector2D pos);

	// Getters
	const Vector2D& getPosition() const;

protected:
	Vector2D mPosition;
};