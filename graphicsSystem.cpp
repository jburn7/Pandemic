#include "graphicsSystem.h"
#include "CameraEvents.h"
#include "SFML\Graphics\RectangleShape.hpp"
#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Graphics\ConvexShape.hpp"

void setPosition(sf::Transformable &trans, const Vector2D &vec)
{
	trans.setPosition(sf::Vector2f(vec.getX(), vec.getY()));
}

GraphicsSystem::GraphicsSystem()
{
	mWidth = 0;
	mHeight = 0;
	mCameraZoom = 1;

	gpEventSystem->addListener(EventType::PLACE_CAMERA_EVENT, this);
	gpEventSystem->addListener(EventType::PAN_CAMERA_EVENT, this);
	gpEventSystem->addListener(EventType::ZOOM_CAMERA_EVENT, this);
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


void GraphicsSystem::drawOutlineForBounds(const sf::FloatRect &bounds, const Outline &outline, const Shape& shape, const double theta)
{
	switch(shape.getType())
	{
		case ShapeType::RECTANGLE:
		{
			sf::RectangleShape boardOutline;
			boardOutline.setSize(sf::Vector2f(bounds.width, bounds.height));

			boardOutline.setPosition(sf::Vector2f(bounds.left, bounds.top));
			boardOutline.setOutlineColor(outline.borderColor.mColor);
			boardOutline.setOutlineThickness((float)outline.thickness);
			boardOutline.setFillColor(outline.fillColor.mColor);

			double degrees = theta * 180.0 / 3.1415926;
			boardOutline.rotate((float)degrees);

			mDisplay.draw(boardOutline);
			break;
		}
		case ShapeType::CIRCLE:
		{
			sf::CircleShape boardOutline;
			boardOutline.setRadius(bounds.width / 2);

			boardOutline.setPosition(sf::Vector2f(bounds.left, bounds.top));
			boardOutline.setOutlineColor(outline.borderColor.mColor);
			boardOutline.setOutlineThickness((float)outline.thickness);
			boardOutline.setFillColor(outline.fillColor.mColor);

			double degrees = theta * 180.0 / 3.1415926;
			boardOutline.rotate((float)degrees);

			mDisplay.draw(boardOutline);
			break;
		}
		case ShapeType::COMPLEX: {
			const std::vector<Vector2D>& vertices = shape.getVertices();
			sf::ConvexShape boardOutline(vertices.size());

			for(unsigned int index = 0; index < vertices.size(); index++)
			{
				boardOutline.setPoint(index, sf::Vector2f(vertices[index].getX(), vertices[index].getY()));
			}

			boardOutline.setPosition(sf::Vector2f(bounds.left, bounds.top));
			boardOutline.setOutlineColor(outline.borderColor.mColor);
			boardOutline.setOutlineThickness((float)outline.thickness);
			boardOutline.setFillColor(outline.fillColor.mColor);

			double degrees = theta * 180.0 / 3.1415926;
			boardOutline.rotate((float)degrees);

			mDisplay.draw(boardOutline);
			break;
		}
	}

}

void GraphicsSystem::draw(const Vector2D &targetLoc, const Sprite &sprite, const Shape &shape, double theta, const Vector2D &scale, const Outline &outline)
{
	sf::Sprite temp;
	const GraphicsBuffer *texture = sprite.getTexture();
	if(texture)
	{
		temp = sf::Sprite(sprite.getTexture()->mBitmap, sf::IntRect((int)sprite.getSourceLoc().getX(), (int)sprite.getSourceLoc().getY(), shape.getWidth(), shape.getHeight()));
	}
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
	temp.setColor(sf::Color(sprite.getColor().mColor));
	temp.setColor(sf::Color(temp.getColor().r, temp.getColor().g, temp.getColor().b, (sf::Uint8)sprite.getTransparency()));

	// Outline drawn here
	drawOutlineForBounds(temp.getGlobalBounds(), outline, shape, theta);
	mDisplay.draw(temp);
}

void GraphicsSystem::drawOutline(const Vector2D& targetLoc, const Vector2D& size, const Outline& outline, double theta)
{
	// Outline drawn here, called in just a couple of places by units that have subunits and no sprite
	sf::FloatRect bounds(sf::Vector2f(targetLoc.getX(), targetLoc.getY()), sf::Vector2f(size.getX(), size.getY()));
	drawOutlineForBounds(bounds, outline, Shape((int)size.getX(), (int)size.getY()), theta);
}

void GraphicsSystem::handleEvent(const Event &theEvent)
{
	switch(theEvent.getType())
	{
	case EventType::PLACE_CAMERA_EVENT:
	{
		mCameraPosition = static_cast<const PlaceCameraEvent&>(theEvent).getPosition();
		break;
	}
	case EventType::PAN_CAMERA_EVENT:
	{
		mCameraPosition += static_cast<const PanCameraEvent&>(theEvent).getDelta();
		break;
	}
	case EventType::ZOOM_CAMERA_EVENT:
	{
		const ZoomCameraEvent& ev = static_cast<const ZoomCameraEvent&>(theEvent);
		mCameraZoom += ev.getDelta();
		mZoomPosition = sf::Vector2i((int)ev.getZoomLocation().getX(), (int)ev.getZoomLocation().getY());
		break;
	}
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
	case GraphicsLayer::BASE_VIEW:
	{
		zoomViewAt(mZoomPosition, mDisplay, (float)mCameraZoom);
		sf::View view = mDisplay.getView();
		mTopLeft = Vector2D(view.getCenter().x - mWidth / 2, view.getCenter().y - mHeight / 2);
	}
		break;
	case GraphicsLayer::GUI_VIEW:
		sf::View view(sf::Vector2f((float)mWidth / 2, (float)mHeight / 2), sf::Vector2f((float)mWidth, (float)mHeight));
		mDisplay.setView(view);
		break;
	}
}

void GraphicsSystem::writeText(const Vector2D &targetLoc, const int fontSize, Font &font, const Color &color, const std::string &message, const Outline &background, const Vector2D &scale){
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

	// Outline drawn here
	drawOutlineForBounds(temp.getGlobalBounds(), background, Shape((int)temp.getGlobalBounds().width, (int)temp.getGlobalBounds().height), 0);
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
