#include "TextControl.h"

TextControl::TextControl()
{
}

TextControl::TextControl(std::string name, int width, int height)
	: Control(name, width, height)
{
	if (width == 0 && height == 0)
		mAutoSize = true;
}

void TextControl::setAutosize(bool state, int x, int y)
{
	mAutoSize = state;
	mXMargin = x; mYMargin = y;
	if (mAutoSize)
		autoSize();
}

void TextControl::setText(std::string txt) 
{
	Control::setText(txt);
	autoSize();
}

void TextControl::autoSize()
{
	if (!mAutoSize)
		return;
	SIZE size;
	GetTextExtentPoint32(GetDC(mHwnd), mText.c_str(), mText.size(), &size);
	setSize(size.cx + mXMargin, size.cy + mYMargin);
}
