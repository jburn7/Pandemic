#pragma once
#include "Player.h"
#include "Outline.h"

/*
* A wrapper for the UI element that will show a player's info (i.e. pawn color, role (when we add those), outline indicating whether they are active, another indicator to show when you are viewing their hand)
* Clicking this will show that player's hand
* These will be instantiated once when each Player is created
*/

class PlayerInfo : public Unit
{
public:
	PlayerInfo(Player *player, const Vector2D &position, const float playerSpriteScale, const Outline &selectedIndicator);
	virtual ~PlayerInfo();

	virtual void draw();

	void setSelected(const bool &selected);

	Player *const getPlayer();

private:
	bool mSelected;

	const Outline mSelectedIndicator;
	Player *const mpPlayer;
	TextBox* mpTextBox;
	const float mPlayerSpriteScale;
};