#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

enum Margin 
{ 
	Undefined,
	Left,
	Center,
	Right
};

class TextControl : public Control
{
public:
	void setAutosize(bool, int = 0, int = 0);
	virtual void setMargin(Margin);
	virtual void setText(std::string);
	virtual void setSize(int, int);
	virtual void setRect(RECT);
	virtual Margin getMargin();
protected:
	TextControl();
	TextControl(std::string, int, int);
	HFONT mFont;
	bool mAutoSize = false;
	int mXMargin = 0, mYMargin = 0;

	void autoSize();
};