#include "CheckBox.h"

CheckBox::CheckBox()
{
}

CheckBox::CheckBox(Control* parent, std::string name, RECT rect)
	: Button(parent, name, rect.right, rect.bottom)
{
	setLocation(rect.left, rect.top);
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_AUTOCHECKBOX;
	mType = WC_BUTTON;
	create();
}

CheckBox::CheckBox(Control* parent, std::string name, int width, int height)
	: Button(parent, name, width, height)
{
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_AUTOCHECKBOX;
	mType = WC_BUTTON;
	create();
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

LRESULT CheckBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		mChecked = !mChecked;
		if (mOnCheckChange != nullptr)
			mOnCheckChange(this, mChecked);
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
