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
#include "Outline.h"

enum class GraphicsLayer
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

	void draw(const Vector2D &targetLoc, const Sprite &sprite, double theta = 0, const Vector2D &scale = Vector2D(1, 1), const Outline &outline = Outline());

	virtual void handleEvent(const Event &theEvent);

	void flip();

	//just updates the camera position, does NOT draw
	void update(const GraphicsLayer layer);

	void writeText(const Vector2D &targetLoc, const int fontSize, Font &font, Color &color, std::string &message, const Outline &background = Outline(), const Vector2D &scale = Vector2D(1, 1));

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
	void drawOutlineForBounds(const sf::FloatRect &bounds, const Outline &outline);
	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);
};