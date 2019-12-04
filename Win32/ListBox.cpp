#include "ListBox.h"

ListBox::ListBox()
{
}

ListBox::ListBox(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY;
	mType = WC_LISTBOX;
	create();
}

ListBox::ListBox(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY;
	mType = WC_LISTBOX;
	create();
}

void ListBox::setOnSelect(f_onSelectIndex call)
{
	mOnSelectionChanged = call;
}

void ListBox::select(int idx)
{
	SendMessage(mHwnd, LB_SETCURSEL, idx, 0);
}

void ListBox::select(std::string str)
{
	select(find(str));
}

void ListBox::appendString(std::string str)
{
	SendMessage(mHwnd, LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

void ListBox::removeString(int idx)
{
	SendMessage(mHwnd, LB_DELETESTRING, idx, 0);
}

void ListBox::clear()
{
	SendMessage(mHwnd, LB_RESETCONTENT, 0, 0);
}

int ListBox::find(std::string str)
{
	return SendMessage(mHwnd, LB_FINDSTRING, -1, (LPARAM)str.c_str());
}

std::string ListBox::at(int id)
{
	int nItemLen = (int)SendMessage(mHwnd, LB_GETTEXTLEN, id, 0);
	char* tmp = new char[nItemLen + 1];
	SendMessage(mHwnd, LB_GETTEXT, id, (LPARAM)tmp);
	std::string strResult = tmp;
	delete[] tmp;
	return strResult;
}

int ListBox::size()
{
	return SendMessage(mHwnd, LB_GETCOUNT, 0, 0);
}

int ListBox::selectedIndex()
{
	return SendMessage(mHwnd, LB_GETCURSEL, 0, 0);
}

std::string ListBox::selectedText()
{
	int idx = selectedIndex();
	if (idx == LB_ERR)
		return "";
	return at(idx);
}

LRESULT ListBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case LBN_SELCHANGE:
		if (mOnSelectionChanged)
			mOnSelectionChanged(this, selectedIndex());
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT ListBox::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}