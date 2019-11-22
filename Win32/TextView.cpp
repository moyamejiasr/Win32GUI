#include "TextView.h"

TextView::TextView()
{
}

TextView::TextView(Control* parent, std::string name, RECT rect)
	: Control(parent, name, rect.right, rect.bottom)
{
	setLocation(rect.left, rect.top);
	if (rect.right == 0 && rect.bottom == 0)
		mAutoSize = true;
	mStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;
	mType = WC_STATIC;
	create();
	autosize();
}

TextView::TextView(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	if (width == 0 && height == 0)
		mAutoSize = true;
	mStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;
	mType = WC_STATIC;
	create();
	autosize();
}

void TextView::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void TextView::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void TextView::setEllipsis(bool state)
{
	if (state)
		appendFlag(SS_ENDELLIPSIS);
	else
		removeFlag(SS_ENDELLIPSIS);
}

bool TextView::isEllipsisOn()
{
	return hasFlag(SS_ENDELLIPSIS);
}

void TextView::setAutosize(bool state)
{
	mAutoSize = state;
	autosize();
}

void TextView::setSize(int width, int height)
{
	setAutosize(false);
	Control::setSize(width, height);
	// Fix to the static resize garbage creation
	redraw();
}

void TextView::setRect(RECT rect)
{
	setAutosize(false);
	Control::setRect(rect);
	// Fix to the static resize garbage creation
	redraw();
}

void TextView::setAlign(Align margin)
{
	removeFlag(SS_LEFT);
	removeFlag(SS_CENTER);
	removeFlag(SS_RIGHT);

	if (margin == Align::Left)
		appendFlag(SS_LEFT);
	if (margin == Align::Center)
		appendFlag(SS_CENTER);
	if (margin == Align::Right)
		appendFlag(SS_RIGHT);
}

void TextView::setText(std::string txt)
{
	Control::setText(txt);
	autosize();
}

Align TextView::getAlign()
{
	if (hasFlag(SS_LEFT))
		return Align::Left;
	if (hasFlag(SS_CENTER))
		return Align::Center;
	if (hasFlag(SS_RIGHT))
		return Align::Right;

	return Align::Undefined;
}

void TextView::autosize()
{
	if (!mAutoSize)
		return;
	SIZE size;
	GetTextExtentPoint32(GetDC(mHwnd), mText.c_str(), mText.size(), &size);
	mWidth = size.cx; mHeight = size.cy;
	setSize(mWidth, mHeight);
}

LRESULT TextView::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case STN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this);
		break;
	case STN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT TextView::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, RGB(255, 0, 0));
	return (LRESULT)mBkBrush;
}
