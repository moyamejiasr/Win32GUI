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
}

void TabControl::appendPage(std::string str)
{
	TCITEM item{};
	item.mask = TCIF_TEXT;
	item.pszText = (LPSTR)str.c_str();
	mPages.push_back(item);

	if (mCreated)
		SendMessageW(mHwnd, TCM_INSERTITEM, mPages.size()-1, (LPARAM)&item);
}

void TabControl::appendPage(std::string str, int idx)
{
	TCITEM item{};
	item.mask = TCIF_TEXT | TCIF_IMAGE;
	item.pszText = (LPSTR)str.c_str();
	item.iImage = idx;
	mPages.push_back(item);

	if (mCreated)
		SendMessageW(mHwnd, TCM_INSERTITEM, mPages.size() - 1, (LPARAM)& item);
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

LRESULT TabControl::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case TCN_SELCHANGING:
		// Return FALSE to allow the selection to change.
		return FALSE;
	case TCN_SELCHANGE:
		int iPage = TabCtrl_GetCurSel(mHwnd);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT TabControl::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}
