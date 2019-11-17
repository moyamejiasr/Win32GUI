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

void TextControl::setMargin(Margin margin)
{
#ifdef _DEBUG
	std::cout << "undefined setMargin for " << mHwnd << std::endl;
#endif
}

void TextControl::setText(std::string txt) 
{
	Control::setText(txt);
	autoSize();
}

void TextControl::setSize(int width, int height)
{
	setAutosize(false);
	Control::setSize(width, height);
}

void TextControl::setRect(RECT rect)
{
	setAutosize(false);
	Control::setRect(rect);
}

Margin TextControl::getMargin()
{
	return Margin::Undefined;
}

void TextControl::autoSize()
{
	if (!mAutoSize)
		return;
	SIZE size;
	GetTextExtentPoint32(GetDC(mHwnd), mText.c_str(), mText.size(), &size);
	mWidth = size.cx + mXMargin;
	mHeight = size.cy + mYMargin;
	setSize(mWidth, mHeight);
}
