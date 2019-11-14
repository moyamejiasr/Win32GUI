#include "Button.h"

Button::Button()
{
}

Button::Button(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 18; mYMargin = 12;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY;
	mParent = parent;
	mType = WC_BUTTON;
	create();
	autoSize();
}

void Button::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void Button::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void Button::setMargin(Margin margin)
{
	removeFlag(BS_LEFT);
	removeFlag(BS_CENTER);
	removeFlag(BS_RIGHT);

	if (margin == Margin::Left)
		appendFlag(BS_LEFT);
	if (margin == Margin::Center)
		appendFlag(BS_CENTER);
	if (margin == Margin::Right)
		appendFlag(BS_RIGHT);
}

Margin Button::getMargin()
{
	if (hasFlag(BS_LEFT))
		return Margin::Left;
	if (hasFlag(BS_CENTER))
		return Margin::Center;
	if (hasFlag(BS_RIGHT))
		return Margin::Right;

	return Margin::Undefined;
}

Button::Button(std::string name, int width, int height)
	: TextControl(name, width, height)
{
}

LRESULT Button::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case BN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this);
		break;
	case BN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this);
		break;
	}
	return true;
}
