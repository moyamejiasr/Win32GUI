#include "TreeView.h"

TreeView::TreeView()
{
}

TreeView::TreeView(Control* parent, RECT rect)
	: Control(parent, WC_TREEVIEW, rect.right, rect.bottom)
{
	cmnControlInit(ICC_TREEVIEW_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = WC_TREEVIEW;
	create();
}

TreeView::TreeView(Control* parent, int width, int height)
	: Control(parent, WC_TREEVIEW, width, height)
{
	cmnControlInit(ICC_TREEVIEW_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = WC_TREEVIEW;
	create();
}

void TreeView::setOnSelect(f_onSelectItem call)
{
	mOnSelectionChanged = call;
}

void TreeView::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void TreeView::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void TreeView::setTextColor(COLORREF color)
{
	mFtColor = color;
	if (mCreated)
		SendMessage(mHwnd, TVM_SETTEXTCOLOR, 0, mFtColor);
}

void TreeView::setBackground(COLORREF color)
{
	SendMessage(mHwnd, TVM_SETBKCOLOR, 0, color);
}

void TreeView::setButtons(bool state)
{
	if (state)
		appendFlag(TVS_HASBUTTONS | TVS_LINESATROOT);
	else
		removeFlag(TVS_HASBUTTONS | TVS_LINESATROOT);
}

void TreeView::setFullSelect(bool state)
{
	if (state)
		appendFlag(TVS_FULLROWSELECT);
	else
		removeFlag(TVS_FULLROWSELECT);
}

void TreeView::setAutoExpand(bool state)
{
	if (state)
		appendFlag(TVS_SINGLEEXPAND);
	else
		removeFlag(TVS_SINGLEEXPAND);
}

void TreeView::setImageList(ImageList* img)
{
	SendMessage(mHwnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)img->handler());
}

void TreeView::select(TreeItem* item)
{
	if (item)
		SendMessage(mHwnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item->mHandler);
}

void TreeView::appendItem(TreeItem* item)
{
	item->mHandler = (HTREEITEM)SendMessage(mHwnd, TVM_INSERTITEM,
		0, (LPARAM)&item->mStruct);
	if (item->mHandler)
		mItems[item->mHandler] = item;
}

void TreeView::removeItem(TreeItem* item)
{
	if (item != nullptr && mCreated)
		if (SendMessage(mHwnd, TVM_DELETEITEM, 0, (LPARAM)item->mHandler))
			mItems.erase(item->mHandler);
}

void TreeView::sortChildrens()
{
	if (mCreated)
		SendMessage(mHwnd, TVM_SORTCHILDREN, FALSE, (LPARAM)TVI_ROOT);
}

void TreeView::clear()
{
	if (mCreated)
		SendMessage(mHwnd, TVM_DELETEITEM, 0, NULL);
}

TreeItem* TreeView::hitTest()
{
	TVHITTESTINFO info{ 0 };
	GetCursorPos(&info.pt);
	ScreenToClient(mHwnd, &info.pt);
	SendMessage(mHwnd, TVM_HITTEST, 0, (LPARAM)&info);
	if (mItems.find(info.hItem) != mItems.end())
		return mItems[info.hItem];
	return nullptr;
}

TreeItem* TreeView::selectedItem()
{
	if (!mCreated)
		return nullptr;
	HTREEITEM handler = (HTREEITEM)SendMessage(mHwnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
	if (mItems.find(handler) != mItems.end())
		return mItems[handler];
	return nullptr;
}

size_t TreeView::size()
{
	return mItems.size();
}

LRESULT TreeView::cnotify(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case TVN_SELCHANGED:
	{
		TreeItem* item = selectedItem();
		if (mOnSelectionChanged)
			mOnSelectionChanged(this, (LPARAM)item);
		break;
	}
	case NM_CLICK:
	{
		if (mOnClick)
			mOnClick(this, MouseKeys::LeftButton);
		break;
	}
	case NM_RCLICK:
	{
		HTREEITEM hItem = TreeView_GetNextItem(mHwnd, 0, TVGN_DROPHILITE);
		if (hItem)
			TreeView_SelectItem(mHwnd, hItem);

		if (mOnClick)
			mOnClick(this, MouseKeys::RightButton);
		break;
	}
	case NM_DBLCLK:
	{
		if (mOnDoubleClick)
			mOnDoubleClick(this, MouseKeys::LeftButton);
		break;
	}
	case NM_RDBLCLK:
	{
		HTREEITEM hItem = TreeView_GetNextItem(mHwnd, 0, TVGN_DROPHILITE);
		if (hItem)
			TreeView_SelectItem(mHwnd, hItem);

		if (mOnDoubleClick)
			mOnDoubleClick(this, MouseKeys::RightButton);
		break;
	}
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

TreeItem::TreeItem()
{
}

TreeItem::TreeItem(TreeItem* parent, std::string name, int normal, int selected)
{
	mParent = parent;
	if (mParent != nullptr)
		mStruct.hParent = parent->mHandler;
	mStruct.hInsertAfter = TVI_LAST;

	mStruct.item.mask |= TVIF_TEXT;
	mText = name;
	mStruct.item.pszText = (LPTSTR)mText.c_str();

	if (normal > -1)
	{
		mStruct.item.mask |= TVIF_IMAGE;
		mStruct.item.iImage = normal;
	}
	if (selected > -1)
	{
		mStruct.item.mask |= TVIF_SELECTEDIMAGE;
		mStruct.item.iSelectedImage = selected;
	}
}

void TreeItem::setParam(LPARAM p)
{
	mParam = p;
}

LPARAM TreeItem::Param()
{
	return mParam;
}

TreeItem* TreeItem::Parent()
{
	return mParent;
}
