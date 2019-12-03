#pragma once
#pragma warning (disable : 26495)

#include "Button.h"

class RadioButton : public Button
{
public:
	RadioButton();
	RadioButton(Control*, std::string, bool, RECT);
	RadioButton(Control*, std::string, bool, int = 0, int = 0);

	void check();
	void uncheck();
	void setOnCheckChange(f_onCheckChange);
	void setChecked(bool);
protected:
	f_onCheckChange mOnCheckChange;
	bool mChecked = false;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};