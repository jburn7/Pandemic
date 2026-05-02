#pragma once
#include "Moveable.h"

class Camera : public Moveable
{
	friend class CameraManager;
public:
	Camera();

	// Setters
	virtual void setPosition(Vector2D pos);

	// Getters
	const float getZoom() const;

private:
	float mZoom;
};