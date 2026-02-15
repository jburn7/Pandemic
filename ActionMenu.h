#pragma once
#include "textBox.h"

/**
* TODO: few ways to go about this. We can have an int super enum of every menu action
* Or we can have a super string enum of every action, then we don't need MenuAction struct
* Or we can store an ActionMenuType in Board, and then Board maps the index of selected action based on which type
* Or ActionMenu can store type itself and then it doesn't need to accept string initializer, but that would still need one of two enum approaches above
*/
enum MenuActionType
{
	CHARTER_FLIGHT,
	BUILD_RESEARCH_STATION,
	NONE
};

struct MenuAction
{
	std::string name;
	MenuActionType type;
};

class ActionMenu : public Unit
{
public:	
	ActionMenu(const Vector2D& position, const std::initializer_list<MenuActionType> actions, const int maxWidth = 0);
	~ActionMenu();

	virtual void draw() const override;

	MenuActionType handleClick(const Vector2D position);

	Unit* getContainingMenuItem(Vector2D guiPos);
	virtual int getWidth() const;
	virtual int getHeight() const;
private:
	static std::string sMenuActionNamesByType[];
	std::vector<TextBox*> mMenuItems;
	std::vector<MenuActionType> mActionTypes;

	int mWidth;
	int mHeight;
};