#pragma once
#pragma warning (disable : 26495)

#define _AFXDLL
#include <SDKDDKVer.h>
#include <afxwin.h>

#include <string>
#include <vector>

class ControlMenu;
class ContextMenu;

class MenuItem
{
public:
	MenuItem();
	MenuItem(std::string);

	void setSeparator(bool);
	void setEnabled(bool);
	void setText(std::string);
	void setBitmap(HBITMAP);
	void setSubMenu(ContextMenu*);
protected:
	friend class MenuBuilder;
	MENUITEMINFO mInfo{};
	std::string mText;
};

class MenuBuilder
{
public:
	void destroy();
	void insert(int, MenuItem);
	void insert(MenuItem);
	HMENU getHMenu();
protected:
	MenuBuilder();
	HMENU mHMenu = nullptr;
	int mCount = 0;
};

class ControlMenu : public MenuBuilder
{
public:
	ControlMenu();
protected:
};

class ContextMenu : public MenuBuilder
{
public:
	ContextMenu();
protected:
	friend class MenuItem;
};