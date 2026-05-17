#pragma once
#include "Moveable.h"

class Camera : public Moveable
{
public:
	Camera();

	// Setters
	virtual void setPosition(Vector2D pos);

	void setZoom(const float zoom, const Vector2D& position);

	// Getters
	const float getZoom() const;

private:
	float mZoom;
};