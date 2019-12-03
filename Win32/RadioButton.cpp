#include "RadioButton.h"

RadioButton::RadioButton()
{
}

RadioButton::RadioButton(Control* parent, std::string name, bool ngroup, RECT rect)
	: Button(name, rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_AUTORADIOBUTTON;
	if (ngroup)
		mStyle |= WS_GROUP;
	mType = WC_BUTTON;
	create();
}

RadioButton::RadioButton(Control* parent, std::string name, bool ngroup, int width, int height)
	: Button(name, width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_AUTORADIOBUTTON;
	if (ngroup)
		mStyle |= WS_GROUP;
	mType = WC_BUTTON;
	create();
}

void RadioButton::check()
{
	setChecked(true);
}

void RadioButton::uncheck()
{
	setChecked(false);
}

void RadioButton::setOnCheckChange(f_onCheckChange call)
{
	mOnCheckChange = call;
}

void RadioButton::setChecked(bool state)
{
	mChecked = state;
	if (mCreated)
		SendMessage(mHwnd, BM_SETCHECK,
			mChecked ? BST_CHECKED : BST_UNCHECKED, 0);
}

LRESULT RadioButton::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		mChecked = !mChecked;
		if (mOnCheckChange != nullptr)
			mOnCheckChange(this, mChecked);
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
