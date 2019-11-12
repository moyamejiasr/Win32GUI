#include "CheckBox.h"

CheckBox::CheckBox()
{
}

CheckBox::CheckBox(Control* parent, std::string name, int width, int height)
	: Button(name, width, height)
{
	mXMargin = 18; mYMargin = 0;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_AUTOCHECKBOX;
	mParent = parent;
	mType = WC_BUTTON;
	create();
	autoSize();
}

void CheckBox::check()
{
	setChecked(true);
}

void CheckBox::uncheck()
{
	setChecked(false);
}

void CheckBox::setOnCheckChange(f_onCheckChange call)
{
	mOnCheckChange = call;
}

void CheckBox::setChecked(bool state)
{
	mChecked = state;
	if (mCreated)
		SendMessage(mHwnd, BM_SETCHECK, 
			mChecked ? BST_CHECKED : BST_UNCHECKED, 0);
}

void CheckBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		mChecked = !mChecked;
		if (mOnCheckChange != nullptr)
			mOnCheckChange(this, mChecked);
	}
}
