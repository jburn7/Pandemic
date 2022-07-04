#pragma once
#include "SFML\Graphics\RenderWindow.hpp"
#include "SFML\Graphics\Sprite.hpp"
#include "SFML\Graphics\Text.hpp"
#include "sprite.h"
#include "font.h"
#include "EventListener.h"
#include "EventSystem.h"
#include <string>
#include "document.h"

enum GraphicsLayer
{
	BASE_VIEW,
	GUI_VIEW
};

class GraphicsSystem : public EventListener
{
	friend class InputSystem; //for polling RenderWindow Events
public:
	GraphicsSystem();

	~GraphicsSystem();

	void init(const rapidjson::Document &doc, int w, int h, const std::string &title);

	void cleanup();

	void clear();

	Vector2D convertToWorldCoordinates(Vector2D pos, const GraphicsLayer layer);

	void draw(const Vector2D &targetLoc, Sprite &sprite);

	void draw(const Vector2D &targetLoc, Sprite &sprite, double theta, const Vector2D &scale);

	void drawScale(const Vector2D &targetLoc, Sprite &sprite, const Vector2D &scale);

	virtual void handleEvent(const Event &theEvent);

	void flip();

	//just updates the camera position, does NOT draw
	void update(const GraphicsLayer layer);

	void writeText(const Vector2D &targetLoc, const int fontSize, Font &font, Color &color, std::string &message);

	//setters and getters
	void setHeight(int h);

	void setWidth(int w);

	int getHeight();

	int getWidth();

	const Vector2D &getTopLeft() const;
protected:
	int mWidth, mHeight;

	Vector2D mCameraPosition;
	sf::Vector2i oldOffsetCoords;
	sf::Vector2i mZoomPosition;
	double mCameraZoom;
	Vector2D mTopLeft; // For consistent UI drawing after camera has panned
	sf::RenderWindow mDisplay;
	sf::FloatRect mBoardViewport;

private:
	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);
};