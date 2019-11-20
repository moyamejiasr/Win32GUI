#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class TextView : public Control
{
public:
	TextView();
	TextView(Control*, std::string, RECT);
	TextView(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	void setEllipsis(bool);
	bool isEllipsisOn();
	void setAutosize(bool);
	void setAlign(Align);
	virtual void setText(std::string);
	virtual void setSize(int, int);
	virtual void setRect(RECT);
	Align getAlign();
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;
	bool mAutoSize = false;

	void autosize();
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};