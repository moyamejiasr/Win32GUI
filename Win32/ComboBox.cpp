#include "ComboBox.h"

ComboBox::ComboBox()
{
}

ComboBox::ComboBox(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWN;
	mType = WC_COMBOBOX;
	create();
}

ComboBox::ComboBox(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWN;
	mType = WC_COMBOBOX;
	create();
}

void ComboBox::setOnFocus(f_onFocus call)
{
	mOnFocus = call;
}

void ComboBox::setOnSelect(f_onSelectIndex call)
{
	mOnSelectionChanged = call;
}

void ComboBox::showDropDown()
{
	if (mCreated)
		SendMessage(mHwnd, CB_SHOWDROPDOWN, TRUE, NULL);
}

void ComboBox::select(int val)
{
	if (mCreated)
		SendMessage(mHwnd, CB_SETCURSEL, val, NULL);
}

void ComboBox::select(std::string str)
{
	if (mCreated)
		SendMessage(mHwnd, CB_SELECTSTRING, -1, (LPARAM)str.c_str());
}

LRESULT ComboBox::find(std::string str)
{
	if (mCreated)
		return SendMessage(mHwnd, CB_FINDSTRING, -1, (LPARAM)str.c_str());
	return -1;
}

std::string ComboBox::at(int index)
{
	if (!mCreated)
		return "";
	LRESULT len = SendMessage(mHwnd, CB_GETLBTEXTLEN, index, NULL) + 1;
	TCHAR* buff = new TCHAR[len];
	SendMessage(mHwnd, CB_GETLBTEXT, index, (LPARAM)buff);
	std::string result = buff;
	delete[] buff;
	return result;
}

void ComboBox::appendString(std::string str)
{
	if (mCreated)
		SendMessage(mHwnd, CB_ADDSTRING, NULL, (LPARAM)str.c_str());
}

void ComboBox::removeString(int val)
{
	if (mCreated)
		SendMessage(mHwnd, CB_DELETESTRING, val, NULL);
}

LRESULT ComboBox::size()
{
	if (mCreated)
		return SendMessage(mHwnd, CB_GETCOUNT, NULL, NULL);
	return 0;
}

LRESULT ComboBox::selectedIndex()
{
	if (mCreated)
		return SendMessage(mHwnd, CB_GETCURSEL, NULL, NULL);
	return -1;
}

std::string ComboBox::selectedText()
{
	if (mCreated)
		return at(selectedIndex());
	return "";
}

ComboBox::ComboBox(Control* parent, DWORD flag, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | flag;
	mType = WC_COMBOBOX;
	create();
}


LRESULT ComboBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case CBN_SETFOCUS:
		if (mOnFocus)
			mOnFocus(this, true);
		break;
	case CBN_KILLFOCUS:
		if (mOnFocus)
			mOnFocus(this, false);
		break;
	case CBN_SELCHANGE:
		if (mOnSelectionChanged)
			mOnSelectionChanged(this, selectedIndex());
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
