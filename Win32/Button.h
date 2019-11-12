#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

using f_onClick = void(*)(Button*);
using f_onDoubleClick = void(*)(Button*);

class Button : public TextControl
{
public:
	Button();
	Button(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;

	Button(std::string, int = 0, int = 0);
	virtual void execute(UINT, WPARAM, LPARAM);
};