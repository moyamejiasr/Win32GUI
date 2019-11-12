#include "EditText.h"

EditText::EditText()
{
}

EditText::EditText(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 0; mYMargin = 6;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY;
	mParent = parent;
	mType = WC_EDIT;
	create();
	setCharLimit(NULL);
	autoSize();
}

void EditText::setCharLimit(int val)
{
	mCharLimit = val;
	if (mCreated)
		SendMessage(mHwnd, EM_SETLIMITTEXT, mCharLimit, NULL);
}

void EditText::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == EN_CHANGE)
	{
		mText = pullWindowText();
		autoSize();
	}
}