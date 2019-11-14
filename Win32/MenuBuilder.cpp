#include "MenuBuilder.h"

MenuItem::MenuItem()
{
	mInfo.cbSize = sizeof(MENUITEMINFO);
	mInfo.fMask = MIIM_TYPE | MIIM_ID;
}

MenuItem::MenuItem(std::string str)
{
	mInfo.cbSize = sizeof(MENUITEMINFO);
	mInfo.fMask = MIIM_TYPE | MIIM_ID;
	setText(str);
}

void MenuItem::setSeparator(bool state)
{
	if (state)
		mInfo.fType |= MFT_SEPARATOR;
	else
		mInfo.fType &= ~MFT_SEPARATOR;
}

void MenuItem::setEnabled(bool state)
{
	if (state)
		mInfo.fType |= MFS_ENABLED;
	else
		mInfo.fType &= ~MFS_DISABLED;
}

void MenuItem::setText(std::string str)
{
	mInfo.fType |= MFT_STRING;
	mText = str;
	mInfo.dwTypeData = (LPSTR)mText.c_str();
	mInfo.cch = mText.length();
}

void MenuItem::setBitmap(HBITMAP bmp)
{
	mInfo.fMask |= MIIM_CHECKMARKS;
	mInfo.hbmpChecked = bmp;
	mInfo.hbmpUnchecked = bmp;
}

void MenuItem::setSubMenu(ContextMenu* menu)
{
	mInfo.fMask = MIIM_SUBMENU;
	mInfo.hSubMenu = menu->mHMenu;
}

MenuBuilder::MenuBuilder()
{
}

void MenuBuilder::destroy()
{
	DestroyMenu(mHMenu);
}

void MenuBuilder::insert(int pos, MenuItem item)
{
	InsertMenuItem(mHMenu, pos, TRUE, &item.mInfo);
}

void MenuBuilder::insert(MenuItem item)
{
	insert(mCount, item);
}

HMENU MenuBuilder::getHMenu()
{
	return mHMenu;
}

ControlMenu::ControlMenu()
{
	mHMenu = CreateMenu();
}

ContextMenu::ContextMenu()
{
	mHMenu = CreatePopupMenu();
}
