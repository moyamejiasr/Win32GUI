#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class TextControl : public Control
{
public:
	TextControl();
	TextControl(std::string, int, int);

	void setAutosize(bool, int = 0, int = 0);
	virtual void setText(std::string);
protected:
	bool mAutoSize = false;
	int mXMargin = 0, mYMargin = 0;

	void autoSize();
};