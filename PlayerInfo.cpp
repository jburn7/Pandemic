#include "PlayerInfo.h"
#include "game.h"

const int DEF_UI_FONT_SIZE = 20; // TODO: remove hardcoded value

PlayerInfo::PlayerInfo(Player* player, const Vector2D &position, const float playerSpriteScale, const Outline& selectedIndicator) :
	Unit(position, (int)((player->getSprite()->getWidth()) * playerSpriteScale), (int)(player->getSprite()->getHeight() * playerSpriteScale)),
	mpPlayer(player),
	mPlayerSpriteScale(playerSpriteScale),
	mSelectedIndicator(selectedIndicator)
{
	mSelected = false;
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::draw()
{
	GraphicsSystem& graphics = Game::getInstance()->getGraphics();
	// TODO: get width and height of text and take max dimensions of that and sprite
	if(mSelected)
	{
		graphics.drawOutline(mPosition + Vector2D((float)mSelectedIndicator.thickness, (float)mSelectedIndicator.thickness), Vector2D((float)mpPlayer->mConstantFrame->getWidth(), (float)mpPlayer->mConstantFrame->getHeight()), mSelectedIndicator);
	}
	graphics.draw(mPosition, *mpPlayer->mConstantFrame, 0, Vector2D(mPlayerSpriteScale, mPlayerSpriteScale));
	Color color = Color();
	graphics.writeText(mPosition + Vector2D(0, mpPlayer->mConstantFrame->getHeight() * mPlayerSpriteScale), DEF_UI_FONT_SIZE, Game::getInstance()->getDefaultFont(), color, "Role: TODO");
}

void PlayerInfo::setSelected(const bool &selected)
{
	mSelected = selected;
}

 Player *const PlayerInfo::getPlayer()
{
	return mpPlayer;
}
