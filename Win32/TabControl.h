#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class TabControl : public Control
{
public:
	TabControl();
	TabControl(Control*, RECT);
	TabControl(Control*, int = 0, int = 0);

	void appendPage(std::string);
	void appendPage(std::string, int);
	void setOnSelect(f_onSelectIndex);
	void removePage(int);
	void removeAll();
	void select(int);
	void setImageList(ImageList*);
	void setBottomTabs(bool);
	std::string at(int);
	int size();
	int selectedIndex();
	std::string selectedText();
	virtual void setSize(int, int);
	virtual void setRect(RECT);
protected:
	f_onSelectIndex mOnSelectionChanged = nullptr;
	std::vector<TCITEM> mPages;

	void adjustClientRect();
	virtual LRESULT cnotify(UINT, WPARAM, LPARAM);
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};