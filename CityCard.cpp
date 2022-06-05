#include "CityCard.h"
#include "game.h"
#include "ColorManager.h"

CityCard::CityCard(Vector2D pos, Sprite *s, City *city) : Unit(pos, s)
{
	mpCity = city;

	rapidjson::Document &doc = JSONData::getInstance()->getJSON();
	rapidjson::Value &c = doc["cityCard"];
	GraphicsBufferManager *graphics = &Game::getInstance()->getGraphicsBufferManager();
	const Color color = ColorManager::getInstance()->color(c["color"].GetString());

	mText = new UIBox(
		Vector2D(0, 0), 
		(int)(s->getHeight() * 0.15f), 
		Vector2D(0, 0), 
		color, 
		0,
		city->getName(), 
		new Sprite(*graphics->getGraphicsBuffer("blank.png")), 
		nullptr, 
		&Game::getInstance()->getDefaultFont());
	mText->resizeToFitWidth(s->getWidth());
	setPosition(pos);
	setIsHidden(true); // default not to draw until it has been "drawn" from deck (ha)
	setIsGuiLayer(true); // For now, all cards will follow camera like this, not sure whether that will change
	
}

CityCard::~CityCard()
{
	Unit::~Unit();
	if(mText)
	{
		delete mText;
		mText = nullptr;
	}
}

void CityCard::draw()
{
	Unit::draw();
	mText->draw();
}

std::string CityCard::debugDescription()
{
	if(mpCity)
	{
		return mpCity->getName();
	}
	else
	{
		return "";
	}
}

void CityCard::move(const Vector2D & delta)
{
	Unit::move(delta);
	mText->move(delta);
}

void CityCard::setPosition(Vector2D pos)
{
	Unit::setPosition(pos);
	mText->setPosition(pos);
}

City* CityCard::getCity()
{
	return mpCity;
}
