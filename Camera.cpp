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

void Camera::setZoom(const float zoom, const Vector2D& position)
{
	gpEventSystem->fireEvent(new ZoomCameraEvent(zoom - mZoom, position, *this));
	mZoom = zoom;
}

const float Camera::getZoom() const
{
	return mZoom;
}
