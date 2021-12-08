#include "CameraManager.h"
#include "KeyEvent.h"
#include "PanCameraEvent.h"
#include "game.h"

void CameraManager::init(const Vector2D bounds, const float panSpeed)
{
	mCameraPanDown = mCameraPanLeft = mCameraPanRight = mCameraPanUp = false;
	mPanSpeed = panSpeed;
	mBounds = bounds;
	mCenter = Vector2D(0, 0);

	gpEventSystem->addListener(KEY_PRESSED_EVENT, this);
	gpEventSystem->addListener(KEY_RELEASED_EVENT, this);
}

void CameraManager::update()
{
	// TODO: add acceleration
	if(mCameraPanRight)
	{
		const Vector2D delta = Vector2D(std::min(mPanSpeed, std::abs(mBounds.getX() - mCenter.getX())), 0);
		if(mCenter.getX() + delta.getX() <= mBounds.getX())
		{
			gpEventSystem->fireEvent(new PanCameraEvent(PAN_CAMERA_EVENT, delta));
			mCenter += delta;
		}
	}
	if(mCameraPanLeft)
	{
		const Vector2D delta = Vector2D(std::min(-mPanSpeed, std::abs(mCenter.getX() - mBounds.getX())), 0);
		if(mCenter.getX() + delta.getX() >= -mBounds.getX())
		{
			gpEventSystem->fireEvent(new PanCameraEvent(PAN_CAMERA_EVENT, delta));
			mCenter += delta;
		}
	}
	if(mCameraPanUp)
	{
		const Vector2D delta = Vector2D(0, std::min(-mPanSpeed, std::abs(mCenter.getY() - mBounds.getY())));
		if(mCenter.getY() + delta.getY() >= -mBounds.getY())
		{
			gpEventSystem->fireEvent(new PanCameraEvent(PAN_CAMERA_EVENT, delta));
			mCenter += delta;
		}
	}
	if(mCameraPanDown)
	{
		const Vector2D delta = Vector2D(0, std::min(mPanSpeed, std::abs(mBounds.getY() - mCenter.getY())));
		if(mCenter.getY() + delta.getY() <= mBounds.getY())
		{
			gpEventSystem->fireEvent(new PanCameraEvent(PAN_CAMERA_EVENT, delta));
			mCenter += delta;
		}
	}
}

void CameraManager::handleEvent(const Event &theEvent)
{
	const Gamestate gameState = Game::getInstance()->getGamestate();
	if(theEvent.getType() == KEY_PRESSED_EVENT)
	{
		const KeyPressedEvent &ev = static_cast<const KeyPressedEvent&>(theEvent);
		if(gameState == PLAYING)
		{
			switch(ev.getKey())
			{
			case LEFT:
				mCameraPanLeft = true;
				break;
			case RIGHT:
				mCameraPanRight = true;
				break;
			case UP:
				mCameraPanUp = true;
				break;
			case DOWN:
				mCameraPanDown = true;
				break;
			default:
				break;
			}
		}
	}
	else if(theEvent.getType() == KEY_RELEASED_EVENT)
	{
		const KeyReleasedEvent &ev = static_cast<const KeyReleasedEvent&>(theEvent);
		if(gameState == PLAYING)
		{
			switch(ev.getKey())
			{
			case LEFT:
				mCameraPanLeft = false;
				break;
			case RIGHT:
				mCameraPanRight = false;
				break;
			case UP:
				mCameraPanUp = false;
				break;
			case DOWN:
				mCameraPanDown = false;
				break;
			default:
				break;
			}
		}
	}
}
