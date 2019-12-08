#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class ComboBox : public Control
{
public:
	ComboBox();
	ComboBox(Control*, RECT);
	ComboBox(Control*, int = 0, int = 25);

	void setOnFocus(f_onFocus);
	void setOnSelect(f_onSelectIndex);
	void showDropDown();
	void select(int);
	void select(std::string);
	void appendString(std::string);
	void removeString(int);
	LRESULT find(std::string);
	std::string at(int);
	LRESULT size();
	LRESULT selectedIndex();
	std::string selectedText();
protected:
	ComboBox(Control*, DWORD, int, int);
	f_onFocus mOnFocus = nullptr;
	f_onSelectIndex mOnSelectionChanged = nullptr;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};