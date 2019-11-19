#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class PictureBox : public Control
{
public:
	PictureBox();
	PictureBox(Control*, std::string, RECT);
	PictureBox(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};