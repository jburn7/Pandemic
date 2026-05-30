#include "Camera.h"
#include "CameraEvents.h"
#include "game.h"
#include "KeyEvent.h"

Camera::Camera()
{
	mCurrentZoom = 1;
	mCameraPanLeft = mCameraPanRight = mCameraPanUp = mCameraPanDown = false;
	mPanSpeed = mMaxPanSpeed = mPanAcceleration = 0.f;
	mZoomPerTick = mMaxZoom = mMinZoom = 0.f;
	mBounds = Vector2D();
}

void Camera::init(const Vector2D bounds, const float panSpeed, const float panAcceleration, const float zoomAmount, const float maxZoom, const float minZoom)
{
	mCameraPanDown = mCameraPanLeft = mCameraPanRight = mCameraPanUp = false;
	mMaxPanSpeed = panSpeed;
	mPanSpeed = 0;
	mPanAcceleration = panAcceleration;
	mZoomPerTick = zoomAmount;
	mMaxZoom = maxZoom;
	mMinZoom = minZoom;
	mBounds = bounds;
	setPosition(Vector2D(0, 0));

	gpEventSystem->addListener(EventType::KEY_PRESSED_EVENT, this);
	gpEventSystem->addListener(EventType::KEY_RELEASED_EVENT, this);
	gpEventSystem->addListener(EventType::MOUSE_WHEEL_EVENT, this);
}

void Camera::update()
{
	if(mCameraPanRight)
	{
		const Vector2D delta = Vector2D(std::min(mPanSpeed, std::abs(mBounds.getX() - getPosition().getX())), 0);
		if(getPosition().getX() + delta.getX() <= mBounds.getX())
		{
			move(delta);
		}
	}
	if(mCameraPanLeft)
	{
		const Vector2D delta = Vector2D(std::min(-mPanSpeed, std::abs(getPosition().getX() - mBounds.getX())), 0);
		if(getPosition().getX() + delta.getX() >= -mBounds.getX())
		{
			move(delta);
		}
	}
	if(mCameraPanUp)
	{
		const Vector2D delta = Vector2D(0, std::min(-mPanSpeed, std::abs(getPosition().getY() - mBounds.getY())));
		if(getPosition().getY() + delta.getY() >= -mBounds.getY())
		{
			move(delta);
		}
	}
	if(mCameraPanDown)
	{
		const Vector2D delta = Vector2D(0, std::min(mPanSpeed, std::abs(mBounds.getY() - getPosition().getY())));
		if(getPosition().getY() + delta.getY() <= mBounds.getY())
		{
			move(delta);
		}
	}

	if(mCameraPanRight || mCameraPanLeft || mCameraPanDown || mCameraPanUp)
	{
		mPanSpeed += mPanAcceleration;
		mPanSpeed = std::min(mPanSpeed, mMaxPanSpeed);
		gpEventSystem->fireEvent(new UpdateCameraEvent(*this));
	}
	else
	{
		mPanSpeed = 0;
	}
}

void Camera::setPosition(Vector2D pos) {
	pos.clampToBounds(mBounds);

	Moveable::setPosition(pos);
	gpEventSystem->fireEvent(new UpdateCameraEvent(*this));
}

void Camera::handleEvent(const Event& theEvent)
{
	const Gamestate gameState = Game::getInstance()->getGamestate();
	if(theEvent.getType() == EventType::KEY_PRESSED_EVENT)
	{
		const KeyPressedEvent& ev = static_cast<const KeyPressedEvent&>(theEvent);
		if(gameState == Gamestate::PLAYING)
		{
			switch(ev.getKey())
			{
			case Key::LEFT:
				mCameraPanLeft = true;
				break;
			case Key::RIGHT:
				mCameraPanRight = true;
				break;
			case Key::UP:
				mCameraPanUp = true;
				break;
			case Key::DOWN:
				mCameraPanDown = true;
				break;
			default:
				break;
			}
		}
	}
	else if(theEvent.getType() == EventType::KEY_RELEASED_EVENT)
	{
		const KeyReleasedEvent& ev = static_cast<const KeyReleasedEvent&>(theEvent);
		if(gameState == Gamestate::PLAYING)
		{
			switch(ev.getKey())
			{
			case Key::LEFT:
				mCameraPanLeft = false;
				break;
			case Key::RIGHT:
				mCameraPanRight = false;
				break;
			case Key::UP:
				mCameraPanUp = false;
				break;
			case Key::DOWN:
				mCameraPanDown = false;
				break;
			default:
				break;
			}
		}
	}
	else if(theEvent.getType() == EventType::MOUSE_WHEEL_EVENT)
	{
		const MouseWheelEvent& ev = static_cast<const MouseWheelEvent&>(theEvent);
		const Vector2D zoomLocation = ev.getCursorLocation();
		if(gameState == Gamestate::PLAYING)
		{
			switch(ev.getDirection())
			{
			case MouseWheel::DOWN:
				if(getCurrentZoom() < mMaxZoom)
				{
					setZoom(std::min(getCurrentZoom() + mZoomPerTick, mMaxZoom), zoomLocation);
				}
				break;
			case MouseWheel::UP:
				if(getCurrentZoom() > mMinZoom)
				{
					setZoom(std::max(getCurrentZoom() - mZoomPerTick, mMinZoom), zoomLocation);
				}
				break;
			}
		}
	}
}

void Camera::setZoom(const float zoom, const Vector2D& position)
{
	gpEventSystem->fireEvent(new ZoomCameraEvent(zoom - mCurrentZoom, zoom, position, *this));
	mCurrentZoom = zoom;
}

const float Camera::getCurrentZoom() const
{
	return mCurrentZoom;
}