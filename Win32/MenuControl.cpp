#include "MenuControl.h"

MenuItem::MenuItem(int id)
{
	mInfo.cbSize = sizeof(MENUITEMINFO);
	mInfo.fMask = MIIM_TYPE | MIIM_ID;
	mInfo.wID = id;
	setEnabled(true);
	setSeparator(true);
}

MenuItem::MenuItem(int id, std::string str)
{
	mInfo.cbSize = sizeof(MENUITEMINFO);
	mInfo.fMask = MIIM_TYPE | MIIM_ID;
	mInfo.wID = id;
	setEnabled(true);
	setText(str);
}

void MenuItem::setSeparator(bool state)
{
	if (state)
	{
		mInfo.fMask |= MIIM_TYPE;
		mInfo.fType |= MFT_SEPARATOR;
	}
	else
		mInfo.fType &= ~MFT_SEPARATOR;

	updateChanges();
}

void MenuItem::setEnabled(bool state)
{
	if (state)
	{
		mInfo.fMask |= MIIM_STATE;
		mInfo.fState |= MFS_ENABLED;
		mInfo.fState &= ~MFS_DISABLED;
	}
	else
	{
		mInfo.fMask |= MIIM_STATE;
		mInfo.fState |= MFS_DISABLED;
		mInfo.fState &= ~MFS_ENABLED;
	}

	updateChanges();
}

void MenuItem::setText(std::string str)
{
	mInfo.fType |= MFT_STRING;
	mText = str;
	mInfo.dwTypeData = (LPSTR)mText.c_str();
	mInfo.cch = mText.length();

	updateChanges();
}

void MenuItem::setBitmap(HBITMAP bmp)
{
	if (bmp != nullptr)
		mInfo.fMask |= MIIM_CHECKMARKS;
	else
		mInfo.fMask &= ~MIIM_CHECKMARKS;
	mInfo.hbmpChecked = bmp;
	mInfo.hbmpUnchecked = bmp;

	updateChanges();
}

void MenuItem::setSubMenu(ContextMenuStrip* menu)
{
	mSubMenu = menu;
	if (mSubMenu != nullptr)
	{
		mInfo.fMask |= MIIM_SUBMENU;
		mInfo.hSubMenu = mSubMenu->mHMenu;
	}
	else
	{
		mInfo.fMask &= ~MIIM_SUBMENU;
		mInfo.hSubMenu = NULL;
	}

	updateChanges();
}

bool MenuItem::separator()
{
	return mInfo.fType & MFT_SEPARATOR;
}

bool MenuItem::enabled()
{
	return mInfo.fType & MFS_ENABLED;
}

int MenuItem::getId()
{
	return mInfo.wID;
}

std::string MenuItem::getText()
{
	return mText;
}

HBITMAP MenuItem::getBitmap()
{
	return mInfo.hbmpChecked;
}

ContextMenuStrip* MenuItem::getSubMenu()
{
	return nullptr;
}

void MenuItem::updateChanges()
{
	if (mParent == nullptr)
		return;
	SetMenuItemInfo(mParent->hmenu(), getId(), FALSE, &mInfo);
}

MenuControl::MenuControl()
{
}

MenuControl::~MenuControl()
{
	free();
}

void MenuControl::free()
{
	DestroyMenu(mHMenu);
}

void MenuControl::add(MenuItem* item)
{
	InsertMenuItem(mHMenu, mItems.size(), TRUE, &(item->mInfo));
	item->mParent = this;
	mItems.push_back(item);
}

int MenuControl::size()
{
	return mItems.size();
}

void MenuControl::remove(unsigned int pos)
{
	if (pos >= mItems.size())
		return;
	mItems.erase(mItems.begin() + pos);
	RemoveMenu(mHMenu, pos, FALSE);
}

MenuItem* MenuControl::at(int pos)
{
	return mItems[pos];
}

MenuItem* MenuControl::byId(int id)
{
	for (auto&& item : mItems) if (item->getId() == id)
		return item;
	return nullptr;
}

HMENU MenuControl::hmenu()
{
	return mHMenu;
}

MenuStrip::MenuStrip()
{
	mHMenu = CreateMenu();
}

ContextMenuStrip::ContextMenuStrip()
{
	mHMenu = CreatePopupMenu();
}
