#include "graphicsSystem.h"
#include "CameraEvents.h"
#include "SFML\Graphics\RectangleShape.hpp"

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

void GraphicsSystem::init(const rapidjson::Document &doc, int w, int h, const std::string &t)
{
	std::string title(t);
	mDisplay.create(sf::VideoMode(w, h, 32), sf::String(title));
	mWidth = w;
	mHeight = h;
 	mCameraPosition = Vector2D((float)mWidth / 2, (float)mHeight / 2);
	mZoomPosition = sf::Vector2i((int)mCameraPosition.getX(), (int)mCameraPosition.getY());
	oldOffsetCoords = sf::Vector2i(0, 0);
	sf::View view = sf::View(sf::Vector2f((float)mWidth / 2.f, (float)mHeight / 2.f), sf::Vector2f((float)mWidth, (float)mHeight));
	mBoardViewport = sf::FloatRect(
		doc["window"]["boardViewport"]["startX"].GetFloat(),
		doc["window"]["boardViewport"]["startY"].GetFloat(),
		doc["window"]["boardViewport"]["widthRatio"].GetFloat(),
		doc["window"]["boardViewport"]["heightRatio"].GetFloat());
	view.setViewport(mBoardViewport);
	mDisplay.setView(view);
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
	sf::Vector2f worldPos = mDisplay.mapPixelToCoords(sf::Vector2i((int)pos.getX(), (int)pos.getY()));
	mDisplay.setView(oldView);
	return Vector2D(worldPos.x, worldPos.y);
}

void GraphicsSystem::drawOutlineForBounds(const sf::FloatRect &bounds, const Outline &outline)
{
	sf::RectangleShape boardOutline;
	boardOutline.setSize(sf::Vector2f(bounds.width, bounds.height));
	boardOutline.setPosition(sf::Vector2f(bounds.left, bounds.top));
	boardOutline.setOutlineColor(outline.borderColor.mColor);
	boardOutline.setOutlineThickness((float)outline.thickness);
	boardOutline.setFillColor(outline.fillColor.mColor);
	mDisplay.draw(boardOutline);
}

void GraphicsSystem::draw(const Vector2D &targetLoc, Sprite &sprite, double theta, const Vector2D &scale, const Outline &outline)
{
	sf::Sprite temp;
	GraphicsBuffer *texture = sprite.getTexture();
	if(texture)
	{
		temp = sf::Sprite(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	}
	//sf::Sprite temp(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), (int)sprite.getWidth(), (int)sprite.getHeight()));
	double degrees = theta * 180.0 / 3.1415926;
	const Vector2D offset(targetLoc.getX(), targetLoc.getY());
	setPosition(temp, offset);
	temp.setOrigin(sf::Vector2f(sprite.getOrigin().getX(), sprite.getOrigin().getY()));
	temp.rotate((float)degrees);

	temp.scale(scale.getX(), scale.getY());
	if(scale.getX() < 0)
	{
		temp.move((float)temp.getGlobalBounds().width, 0);
	}
	if(scale.getY() < 0)
	{
		temp.move(0, (float)temp.getGlobalBounds().height);
	}
	if(sprite.getColor())
	{
		temp.setColor(sf::Color(sprite.getColor()->mColor));
	}
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));

	drawOutlineForBounds(temp.getGlobalBounds(), outline);
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
		mZoomPosition = sf::Vector2i((int)ev.getZoomLocation().getX(), (int)ev.getZoomLocation().getY());
	}
}

void GraphicsSystem::flip()
{
	mDisplay.display();
}

void GraphicsSystem::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	sf::Vector2f vec = sf::Vector2f((float)pixel.x, (float)pixel.y);
	sf::View view(sf::Vector2f(mCameraPosition.getX(), mCameraPosition.getY()), sf::Vector2f((float)mWidth, (float)mHeight));
	view.setViewport(mBoardViewport);
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
		zoomViewAt(mZoomPosition, mDisplay, (float)mCameraZoom);
		sf::View view = mDisplay.getView();
		mTopLeft = Vector2D(view.getCenter().x - mWidth / 2, view.getCenter().y - mHeight / 2);
	}
		break;
	case GUI_VIEW:
		sf::View view(sf::Vector2f((float)mWidth / 2, (float)mHeight / 2), sf::Vector2f((float)mWidth, (float)mHeight));
		mDisplay.setView(view);
		break;
	}
}

void GraphicsSystem::writeText(const Vector2D &targetLoc, const int fontSize, Font &font, Color &color, std::string &message, const Outline &background, const Vector2D &scale){
	sf::Text temp(message, font.mFont);
	temp.setOrigin(sf::Vector2f(temp.getLocalBounds().left, temp.getLocalBounds().top));
	setPosition(temp, targetLoc);
	temp.move(0, temp.getLocalBounds().top);
	temp.setFillColor(color.mColor);
	temp.setCharacterSize(fontSize);
	temp.scale(scale.getX(), scale.getY());
	if(scale.getX() < 0)
	{
		temp.move((float)temp.getGlobalBounds().width, 0);
	}
	if(scale.getY() < 0)
	{
		temp.move(0, (float)temp.getGlobalBounds().height);
	}

	drawOutlineForBounds(temp.getGlobalBounds(), background);
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
