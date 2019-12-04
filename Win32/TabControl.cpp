#include "TabControl.h"

TabControl::TabControl()
{
}

TabControl::TabControl(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_TAB_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = WC_TABCONTROL;
	create();
	// Hide legacy dots focus on select
	SendMessage(mHwnd, WM_UPDATEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowSubclass(mHwnd, &SubWndProc, (UINT_PTR)mId, 0);

	adjustClientRect();
}

TabControl::TabControl(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_TAB_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = WC_TABCONTROL;
	create();
	// Hide legacy dots focus on select
	SendMessage(mHwnd, WM_UPDATEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
	SetWindowSubclass(mHwnd, &SubWndProc, (UINT_PTR)mId, 0);

	adjustClientRect();
}

void TabControl::appendPage(std::string str)
{
	TCITEM item{};
	item.mask = TCIF_TEXT;
	item.pszText = (LPSTR)str.c_str();
	mPages.push_back(item);

	if (mCreated)
	{
		SendMessageW(mHwnd, TCM_INSERTITEM, mPages.size() - 1, (LPARAM)& item);
		adjustClientRect();
	}
}

void TabControl::appendPage(std::string str, int idx)
{
	TCITEM item{};
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = (LPSTR)str.c_str();
	item.iImage = idx;
	mPages.push_back(item);

	if (mCreated)
	{
		SendMessageW(mHwnd, TCM_INSERTITEM, mPages.size()-1, (LPARAM)&item);
		adjustClientRect();
	}
}

void TabControl::setOnSelect(f_onSelectIndex call)
{
	mOnSelectionChanged = call;
}

void TabControl::removePage(int index)
{
	mPages.erase(mPages.begin() + index);
	if (mCreated)
		SendMessage(mHwnd, TCM_DELETEITEM, index, 0);
}

void TabControl::removeAll()
{
	mPages.clear();
	if (mCreated)
		SendMessage(mHwnd, TCM_DELETEALLITEMS, 0, 0);
}

void TabControl::select(int index)
{
	if (mCreated)
		SendMessage(mHwnd, TCM_SETCURSEL, index, 0);
}

void TabControl::setImageList(ImageList* list)
{
	SendMessage(mHwnd, TCM_SETIMAGELIST, 0, (LPARAM)list->handler());
}

void TabControl::setBottomTabs(bool state)
{
	if (state)
		appendFlag(TCS_BOTTOM);
	else
		removeFlag(TCS_BOTTOM);
}

std::string TabControl::at(int idx)
{
	return mPages[idx].pszText;
}

int TabControl::size()
{
	return SendMessage(mHwnd, TCM_GETITEMCOUNT, 0, 0);
}

int TabControl::selectedIndex()
{
	return SendMessage(mHwnd, TCM_GETCURSEL, 0, 0);
}

std::string TabControl::selectedText()
{
	return mPages[selectedIndex()].pszText;
}

void TabControl::setSize(int width, int height)
{
	Control::setSize(width, height);
	// Update client rect left
	adjustClientRect();
}

void TabControl::setRect(RECT rect)
{
	Control::setRect(rect);
	// Update client rect left
	adjustClientRect();
}

void TabControl::adjustClientRect()
{
	RECT rect{};
	SendMessage(mHwnd, TCM_ADJUSTRECT, FALSE, (LPARAM)&rect);
	mClientRect.left = rect.left;
	mClientRect.top = rect.top;
}

LRESULT TabControl::cnotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case TCN_SELCHANGE:
	{
		int iPage = selectedIndex();
		if (mOnSelectionChanged)
			mOnSelectionChanged(this, iPage);
		break;
	}
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT TabControl::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}
