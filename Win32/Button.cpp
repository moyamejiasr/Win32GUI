#include "Button.h"

Button::Button()
{
}

Button::Button(Control* parent, std::string name, RECT rect)
	: Control(parent, name, rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY;
	mType = WC_BUTTON;
	create();
}

Button::Button(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY;
	mType = WC_BUTTON;
	create();
}

void Button::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void Button::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void Button::setAlign(Align margin)
{
	removeFlag(BS_LEFT);
	removeFlag(BS_CENTER);
	removeFlag(BS_RIGHT);

	if (margin == Align::Left)
		appendFlag(BS_LEFT);
	if (margin == Align::Center)
		appendFlag(BS_CENTER);
	if (margin == Align::Right)
		appendFlag(BS_RIGHT);
}

void Button::setFlat(bool state)
{
	if (state)
		appendFlag(BS_FLAT);
	else
		removeFlag(BS_FLAT);
}

Align Button::getAlign()
{
	if (hasFlag(BS_LEFT))
		return Align::Left;
	if (hasFlag(BS_CENTER))
		return Align::Center;
	if (hasFlag(BS_RIGHT))
		return Align::Right;

	return Align::Undefined;
}

Button::Button(std::string name, int width, int height)
	: Control(nullptr, name, width, height)
{
}

LRESULT Button::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case BN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this, MouseKeys::LeftButton);
		break;
	case BN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this, MouseKeys::LeftButton);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
