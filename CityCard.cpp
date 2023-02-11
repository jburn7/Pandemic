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
	const Color textColor = ColorManager::getInstance()->color(c["textColor"].GetString());
	mpColorIndicator = new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("blank.png"));
	mpColorIndicator->setHeight(s->getHeight() / 6);
	mpColorIndicator->setWidth(s->getWidth());
	if(city) // Man this is still bad...
	{
		const int cityType = (int)city->getType();
		const rapidjson::Value& colorObject = doc["city"]["infoColorByType"].GetArray()[cityType];
		const Color typeColor = Color(colorObject["r"].GetInt(), colorObject["g"].GetInt(), colorObject["b"].GetInt());
		mpColorIndicator->setColor(typeColor);
	}

	std::string nameText;
	if(city)
	{
		nameText = city->getName();
	}
	else
	{
		// Man this is bad...
		nameText = "Epidemic";
	}
	mText = new UIBox(
		Vector2D(0, 0), 
		(int)(s->getHeight() * 0.15f), 
		textColor, 
		nameText,
		&Game::getInstance()->getDefaultFont());
	mText->resizeToFitWidth((float)s->getWidth());
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
	if(mpColorIndicator)
	{
		delete mpColorIndicator;
		mpColorIndicator = nullptr;
	}
}

void CityCard::draw()
{
	Unit::draw();
	mText->draw();
	// TODO: blank graphics buffer sprite won't draw unless outline fill color is provided, why?
	Game::getInstance()->getGraphics().draw(Vector2D(mPosition.getX(), mPosition.getY() + getHeight() - mpColorIndicator->getHeight() * 1.5f), *mpColorIndicator, mTheta, mScale, Outline(mpColorIndicator->getColor(), mpColorIndicator->getColor()));
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
