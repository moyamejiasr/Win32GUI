#pragma once
#pragma warning (disable : 26495)

#include "Button.h"

using f_onCheckChange = void(*)(CheckBox*, bool);

class CheckBox : public Button
{
public:
	CheckBox();
	CheckBox(Control*, std::string, int = 0, int = 0);

	void check();
	void uncheck();
	void setOnCheckChange(f_onCheckChange);
	void setChecked(bool);
protected:
	f_onCheckChange mOnCheckChange;
	bool mChecked = false;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};