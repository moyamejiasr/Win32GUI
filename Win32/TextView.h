#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

class TextView : public TextControl
{
public:
	TextView();
	TextView(Control*, std::string, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	void setEllipsis(bool);
	bool isEllipsisOn(); 
	virtual void setSize(int, int);
	virtual void setRect(RECT);
	virtual void setMargin(Margin);
	virtual Margin getMargin();
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};