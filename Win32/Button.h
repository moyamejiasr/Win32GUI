#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class Button : public Control
{
public:
	Button();
	Button(Control*, std::string, RECT);
	Button(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	void setAlign(Align);
	Align getAlign();
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;

	Button(std::string, int = 0, int = 0);
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};