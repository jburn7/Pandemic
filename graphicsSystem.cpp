#include "graphicsSystem.h"
#include "CameraEvents.h"

void setPosition(sf::Transformable &trans, const Vector2D &vec)
{
	trans.setPosition(sf::Vector2f(vec.getX(), vec.getY()));
}

GraphicsSystem::GraphicsSystem()
{
	mWidth = 0;
	mHeight = 0;
	mCameraZoom = 1;

	gpEventSystem->addListener(PAN_CAMERA_EVENT, this);
	gpEventSystem->addListener(ZOOM_CAMERA_EVENT, this);
}

GraphicsSystem::~GraphicsSystem()
{
	cleanup();
}

void GraphicsSystem::init(int w, int h, const std::string &t)
{
	std::string title(t);
	mDisplay.create(sf::VideoMode(w, h, 32), sf::String(title));
	mWidth = w;
	mHeight = h;
 	mCameraPosition = Vector2D(mWidth / 2, mHeight / 2);
	mZoomPosition = sf::Vector2i(mCameraPosition.getX(), mCameraPosition.getY());
	oldOffsetCoords = sf::Vector2i(0, 0);
}

void GraphicsSystem::cleanup()
{
	mDisplay.close();
}

void GraphicsSystem::clear()
{
	mDisplay.clear(sf::Color::Black);
}

Vector2D GraphicsSystem::convertToWorldCoordinates(Vector2D pos, const GraphicsLayer layer)
{
	sf::View oldView = mDisplay.getView();
	update(layer); // switch to view corresponding to given layer to get correct coordinate conversions
	sf::Vector2f worldPos = mDisplay.mapPixelToCoords(sf::Vector2i(pos.getX(), pos.getY()));
	mDisplay.setView(oldView);
	return Vector2D(worldPos.x, worldPos.y);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	temp.setPosition(sf::Vector2f(targetLoc.getX(), targetLoc.getY()));
	mDisplay.draw(temp);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, double theta, const Vector2D &scale)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	double degrees = theta * 180.0 / 3.1415926;
	const Vector2D offset(targetLoc.getX(), targetLoc.getY());
	setPosition(temp, offset);
	temp.setOrigin(sf::Vector2f(sprite.getOrigin().getX(), sprite.getOrigin().getY()));
	temp.rotate((float)degrees);



	temp.scale(scale.getX(), scale.getY());
	//only supports unscaled drawing for reflected sprites
	if(scale.getX() == -1)
	{
		temp.move(sprite.getWidth(), 0);
	}
	if(scale.getY() == -1)
	{
		temp.move(0, sprite.getHeight());
	}
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));



	mDisplay.draw(temp);
}

void GraphicsSystem::drawScale(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale)
{
	sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	temp.setPosition(sf::Vector2f(targetLoc.getX(), targetLoc.getY()));
	temp.scale(scale.getX(), scale.getY());
	//only supports unscaled drawing for reflected sprites
	if(scale.getX() == -1)
	{
		temp.move(sprite.getWidth(), 0);
	}
	if(scale.getY() == -1)
	{
		temp.move(0, sprite.getHeight());
	}
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));
	mDisplay.draw(temp);
}

void GraphicsSystem::handleEvent(const Event &theEvent)
{
	if(theEvent.getType() == PAN_CAMERA_EVENT)
	{
		mCameraPosition += static_cast<const PanCameraEvent&>(theEvent).getDelta();
	}
	else if(theEvent.getType() == ZOOM_CAMERA_EVENT)
	{
		const ZoomCameraEvent &ev = static_cast<const ZoomCameraEvent&>(theEvent);
		mCameraZoom += ev.getDelta();
		mZoomPosition = sf::Vector2i(ev.getZoomLocation().getX(), ev.getZoomLocation().getY());
	}
}

void GraphicsSystem::flip()
{
	mDisplay.display();
}

void GraphicsSystem::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	sf::Vector2f vec = sf::Vector2f(pixel.x, pixel.y);
	sf::View view(sf::Vector2f(mCameraPosition.getX(), mCameraPosition.getY()), sf::Vector2f(mWidth, mHeight));
	window.setView(view);
	oldOffsetCoords = pixel;
	const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
	const sf::Vector2f offsetCoords{beforeCoord - afterCoord};

	view.move(offsetCoords);
	window.setView(view);
}

void GraphicsSystem::update(const GraphicsLayer layer)
{
	switch(layer)
	{
	case BASE_VIEW:
	{
		zoomViewAt(mZoomPosition, mDisplay, mCameraZoom);
		sf::View view = mDisplay.getView();
		mTopLeft = Vector2D(view.getCenter().x - mWidth / 2, view.getCenter().y - mHeight / 2);
	}
		break;
	case GUI_VIEW:
		sf::View view(sf::Vector2f(mWidth / 2, mHeight / 2), sf::Vector2f(mWidth, mHeight));
		mDisplay.setView(view);
		break;
	}
}

void GraphicsSystem::writeText(const Vector2D &targetLoc, const int fontSize, Font &font, Color &color, std::string &message){
	sf::Text temp(message, font.mFont);
	setPosition(temp, targetLoc);
	temp.setFillColor(color.mColor);
	temp.setCharacterSize(fontSize);
	mDisplay.draw(temp);
}

void GraphicsSystem::setHeight(int h)
{
	mHeight = h;
}

void GraphicsSystem::setWidth(int w)
{
	mWidth = w;
}

int GraphicsSystem::getHeight()
{
	return mHeight;
}

int GraphicsSystem::getWidth()
{
	return mWidth;
}

const Vector2D & GraphicsSystem::getTopLeft() const 
{
	return mTopLeft;
}
