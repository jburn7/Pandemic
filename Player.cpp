#include "Player.h"
#include "City.h"
#include "game.h"

int Player::playerNum = 0;

Player::Player(City *city, std::vector<PlayerCard*> cards, Sprite *s) : Unit(Vector2D(0, 0), s)
{
	//TODO: fill in rest of cases
	switch(playerNum)
	{
	case 0:
		//TODO: make position data-driven. I might want to constantly render the active player's hand at the same spot on screen and just modify it based on active player
		mHandStart = Vector2D(100, Game::getInstance()->getGraphics().getHeight() - 300);
		mHandDirection = Vector2D(1, 0);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
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

void Player::replaceHandIntoDeck(std::vector<PlayerCard*>& deck, const Vector2D &position)
{
	for(auto &v : mHand)
	{
		v->setPosition(position);
		if(deck.size() > 0)
		{
			deck[0]->setIsHidden(true);
		}
		deck.push_back(v);
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
	const Vector2D offset = Vector2D(0, 0);//Game::getInstance()->getCameraManager().getOffset();
	pc->setPosition(offset + mHandStart + Vector2D(mHandDirection.getX() * (pc->getWidth() + 10) * index, mHandDirection.getY() * (pc->getHeight() + 10) * index));
}
