#include "Camera.h"
#include "CameraEvents.h"
#include "game.h"

Camera::Camera()
{
	mZoom = 1;
}

void Camera::setPosition(Vector2D pos)
{
	Moveable::setPosition(pos);
	gpEventSystem->fireEvent(new UpdateCameraEvent(*this));
}

const float Camera::getZoom() const
{
	return mZoom;
}
