#include "PlayerInfo.h"
#include "game.h"

PlayerInfo::PlayerInfo(Player* player, const Vector2D &position, const float playerSpriteScale, const Outline& selectedIndicator) :
	Unit(position, (int)((player->getSprite()->getWidth()) * playerSpriteScale), (int)(player->getSprite()->getHeight() * playerSpriteScale)),
	mpPlayer(player),
	mPlayerSpriteScale(playerSpriteScale),
	mSelectedIndicator(selectedIndicator)
{
	mSelected = false;

    mpTextBox = new TextBox(position + Vector2D(0, mpPlayer->mConstantFrame->getHeight() * mPlayerSpriteScale), Game::getInstance()->getUI().getFontSize(), Color(), "Role: TODO");
}

PlayerInfo::~PlayerInfo()
{
	if(mpTextBox)
	{
		delete mpTextBox;
		mpTextBox = nullptr;
	}
}

void PlayerInfo::draw()
{
	GraphicsSystem& graphics = Game::getInstance()->getGraphics();
	if(mSelected)
	{
		const Vector2D outlineBounds = Vector2D((float)std::max(mpPlayer->mConstantFrame->getWidth(), mpTextBox->getWidth()), (float)std::max(mpPlayer->mConstantFrame->getHeight(), mpTextBox->getHeight()));
		graphics.drawOutline(mPosition + Vector2D((float)mSelectedIndicator.thickness, (float)mSelectedIndicator.thickness), outlineBounds, mSelectedIndicator);
	}
	graphics.draw(mPosition, *mpPlayer->mConstantFrame, 0, Vector2D(mPlayerSpriteScale, mPlayerSpriteScale));
	mpTextBox->draw();
}

void PlayerInfo::setSelected(const bool &selected)
{
	mSelected = selected;
}

 Player *const PlayerInfo::getPlayer()
{
	return mpPlayer;
}
