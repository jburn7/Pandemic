#include "CityCard.h"
#include "game.h"

CityCard::CityCard(Vector2D pos, Sprite *s, City *city) : Unit(pos, s)
{
	mpCity = city;
	mText = new UIBox(Vector2D(0, 0), s->getWidth() * 0.8f, s->getHeight() * 0.2f, (int)(s->getHeight() * 0.15f), Vector2D(0, 0), city->getName(), new Sprite(*Game::getInstance()->getGraphicsBufferManager().getGraphicsBuffer("blank.png")));
	setPosition(pos);
	setIsHidden(true); // default not to draw until it has been "drawn" from deck (ha)
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
	mText->draw(Game::getInstance()->getDefaultFont(), Color(0, 0, 0));
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
