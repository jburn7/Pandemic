#include "ActionMenu.h"
#include "game.h"

std::string ActionMenu::sMenuActionNamesByType[] = {"Charter Flight", "Build Research Station"};

ActionMenu::ActionMenu(const Vector2D &position, const std::initializer_list<const MenuActionType> actions) : Unit(position, 0, 0), mHeight(0), mActionTypes(actions)
{
	int index = 0;
	const int fontSize = Game::getInstance()->getUI().getFontSize();
	int maxWidth = 0;
	for(auto& action : actions)
	{
		TextBox* const textBox = new TextBox(position + Vector2D(0, (float)(fontSize * index)), fontSize, Color(), sMenuActionNamesByType[action]);
		mMenuItems.push_back(textBox);
		if(textBox->getWidth() > maxWidth)
		{
			maxWidth = textBox->getWidth();
		}
		mHeight += textBox->getHeight();
	}
	// TODO: if not completely within screen, adjust by offset to fit screen

	mWidth = maxWidth;
}

ActionMenu::~ActionMenu()
{
	for(auto& menuItem : mMenuItems)
	{
		delete menuItem;
		menuItem = nullptr;
	}
}

void ActionMenu::draw() const
{
	for(auto textBox : mMenuItems)
	{
		textBox->draw();
	}
}

MenuActionType ActionMenu::handleClick(const Vector2D position)
{
	unsigned int index = 0;
	for(auto& menuItem : mMenuItems)
	{
		if(menuItem->contains(position))
		{
			return mActionTypes[index];
		}
		index++;
	}

	return NONE;
}
