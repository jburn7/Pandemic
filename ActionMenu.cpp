#include "ActionMenu.h"
#include "ColorManager.h"
#include "game.h"

std::string ActionMenu::sMenuActionNamesByType[] = {"Charter Flight", "Build Research Station"};

ActionMenu::ActionMenu(const Vector2D &position, const std::initializer_list<MenuActionType> actions, const int maxWidth) : Unit(position, 0, 0), mHeight(0), mActionTypes(actions)
{
	rapidjson::Document& doc = JSONData::getInstance()->getJSON();
	ColorManager& colorManager = *ColorManager::getInstance();

	setIsGuiLayer(true);

	int index = 0;
	const int fontSize = Game::getInstance()->getUI().getFontSize();
	int maxCalculatedWidth = 0;
	for(auto& action : actions)
	{
		TextBox* const textBox = new TextBox(position + Vector2D(0, (float)(fontSize * index)), fontSize, Color(), sMenuActionNamesByType[action]);
		if(maxWidth > 0)
		{
			textBox->resizeToFitWidth((float)maxWidth);
		}
		mMenuItems.push_back(textBox);
		if(textBox->getWidth() > maxWidth)
		{
			maxCalculatedWidth = textBox->getWidth();
		}
		mHeight += textBox->getHeight();
		index++;
	}

	mWidth = maxCalculatedWidth;

	const float windowWidthOffset = position.getX() + mWidth - Game::getInstance()->getGraphics().getWidth();
	if(windowWidthOffset > 0)
	{
		setPosition(Vector2D(position.getX() - windowWidthOffset, position.getY()));
	}

	const float windowHeightOffset = position.getY() + mHeight - Game::getInstance()->getGraphics().getHeight();
	if(windowHeightOffset > 0)
	{
		setPosition(Vector2D(position.getY(), position.getY() - windowHeightOffset));
	}

	setOutline(Outline(colorManager.color(doc["actionMenu"]["outline"]["color"].GetString()), colorManager.black, doc["actionMenu"]["outline"]["thickness"].GetInt()));
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
	Game::getInstance()->getGraphics().drawOutline(mPosition, Vector2D((float)mWidth, (float)mHeight), mOutline);

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

Unit* ActionMenu::getContainingMenuItem(Vector2D guiPos)
{
	for(auto& menuItem : mMenuItems)
	{
		if(menuItem->contains(guiPos))
		{
			return menuItem;
		}
	}

	return nullptr;
}

int ActionMenu::getWidth() const
{
	return mWidth;
}

int ActionMenu::getHeight() const
{
	return mHeight;
}
