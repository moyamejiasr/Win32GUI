#include "TextView.h"

TextView::TextView()
{
}

TextView::TextView(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 0; mYMargin = 0;
	mStyle = WS_CHILD | WS_VISIBLE | SS_NOTIFY;
	mParent = parent;
	mType = WC_STATIC;
	create();
	autoSize();
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

void TextView::setSize(int width, int height)
{
	// Fix to the static resize garbage creation
	TextControl::setSize(width, height);
	redraw();
}

void TextView::setRect(RECT rect)
{
	// Fix to the static resize garbage creation
	TextControl::setRect(rect);
	redraw();
}

void TextView::setMargin(Margin margin)
{
	removeFlag(SS_LEFT);
	removeFlag(SS_CENTER);
	removeFlag(SS_RIGHT);

	if (margin == Margin::Left)
		appendFlag(SS_LEFT);
	if (margin == Margin::Center)
		appendFlag(SS_CENTER);
	if (margin == Margin::Right)
		appendFlag(SS_RIGHT);
}

Margin TextView::getMargin()
{
	if (hasFlag(SS_LEFT))
		return Margin::Left;
	if (hasFlag(SS_CENTER))
		return Margin::Center;
	if (hasFlag(SS_RIGHT))
		return Margin::Right;

	return Margin::Undefined;
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
	return true;
}