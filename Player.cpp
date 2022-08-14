#include "Player.h"
#include "City.h"
#include "game.h"
#include "PlayerCardDeck.h"

Player::Player(City *city, std::vector<PlayerCard*> cards, const Vector2D &playerHandLocation, Sprite *s) : Unit(Vector2D(0, 0), s)
{
	mHandStart = playerHandLocation;
	mHandDirection = Vector2D((float)1, (float)0);
	mHand = cards;
	moveCity(city);
}

Player::~Player()
{
	Unit::~Unit();
}

void Player::dealCard(PlayerCard *pc)
{
	positionCard(mHand.size(), pc);
	mHand.push_back(pc);
}

void Player::discardCard(PlayerCard *pc)
{
	for(unsigned int i = 0; i < mHand.size(); i++)
	{
		if(mHand[i] == pc)
		{
			mHand.erase(mHand.begin() + i);
			for(unsigned int j = 0; j < mHand.size(); j++) // sure the nested loop is dirty, but do you have a better idea?
			{
				positionCard(j, mHand[j]);
			}
			break;
		}
	}
}

void Player::replaceHandIntoDeck(PlayerCardDeck *deck)
{
	for(auto &v : mHand)
	{
		deck->addCard(v);
	}

	mHand.clear();
}

bool Player::isInCity(City *city)
{
	return currentCity == city;
}

void Player::moveCity(City* const newCity)
{
	if(currentCity)
	{
		currentCity->removePlayer(this);
	}
	newCity->addPlayer(this);
	currentCity = newCity;
}

City *Player::getCurrentCity()
{
	return currentCity;
}

std::vector<PlayerCard*>& Player::getHand()
{
	return mHand;
}

void Player::positionCard(int index, PlayerCard *pc)
{
	const Vector2D offset = Vector2D(0, 0);
	pc->setPosition(offset + mHandStart + Vector2D(mHandDirection.getX() * (pc->getWidth() + 10) * index, mHandDirection.getY() * (pc->getHeight() + 10) * index));
}
