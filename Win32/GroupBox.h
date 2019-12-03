#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class GroupBox : public Control
{
public:
	GroupBox();
	GroupBox(Control*, std::string, RECT);
	GroupBox(Control*, std::string, int = 0, int = 0);

protected:
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};