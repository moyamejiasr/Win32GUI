#pragma once
#pragma warning (disable : 26495)

#define _AFXDLL
#include <SDKDDKVer.h>
#include <afxwin.h>

#include <string>
#include <vector>

class MenuControl;
class MenuStrip;
class ContextMenuStrip;

class MenuItem
{
public:
	MenuItem(int);
	MenuItem(int, std::string);

	void setSeparator(bool);
	void setEnabled(bool);
	void setText(std::string);
	void setBitmap(HBITMAP);
	void setSubMenu(ContextMenuStrip*);
	bool separator();
	bool enabled();
	int getId();
	std::string getText();
	HBITMAP getBitmap();
	ContextMenuStrip* getSubMenu();
protected:
	friend class MenuControl;
	MENUITEMINFO mInfo{};
	std::string mText;
	MenuControl* mParent = nullptr;
	ContextMenuStrip* mSubMenu = nullptr;

	void updateChanges();
};

class MenuControl
{
public:
	~MenuControl();

	void free();
	void add(MenuItem*);
	void remove(unsigned int);
	int size();
	MenuItem* at(int);
	MenuItem* byId(int);
	HMENU hmenu();
protected:
	MenuControl();
	HMENU mHMenu = nullptr;
	std::vector<MenuItem*> mItems;
};

class MenuStrip : public MenuControl
{
public:
	MenuStrip();
protected:
};

class ContextMenuStrip : public MenuControl
{
public:
	ContextMenuStrip();
protected:
	friend class MenuItem;
};