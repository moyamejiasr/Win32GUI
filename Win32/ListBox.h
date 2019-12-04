#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class ListBox : public Control
{
public:
	ListBox();
	ListBox(Control*, RECT);
	ListBox(Control*, int = 0, int = 0);

	void setOnSelect(f_onSelectIndex);
	void select(int);
	void select(std::string);
	void appendString(std::string);
	void removeString(int);
	void clear();
	int find(std::string);
	std::string at(int);
	int size();
	int selectedIndex();
	std::string selectedText();
protected:
	f_onSelectIndex mOnSelectionChanged = nullptr;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};