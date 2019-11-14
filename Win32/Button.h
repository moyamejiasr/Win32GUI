#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

class Button : public TextControl
{
public:
	Button();
	Button(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	virtual void setMargin(Margin);
	virtual Margin getMargin();
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;

	Button(std::string, int = 0, int = 0);
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};