#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class TreeItem
{
public:
	TreeItem();
	TreeItem(TreeItem*, std::string, int = -1, int = -1);
	void setParam(LPARAM);
	LPARAM Param();
	TreeItem* Parent();
protected:
	friend class TreeView;
	TreeItem* mParent;
	std::string mText;
	HTREEITEM mHandler = nullptr;
	LPARAM mParam = NULL;
	TVINSERTSTRUCT mStruct{ 0 };
};

class TreeView : public Control
{
public:
	TreeView();
	TreeView(Control*, RECT);
	TreeView(Control*, int = 0, int = 0);

	void setOnSelect(f_onSelectItem);
	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	virtual void setTextColor(COLORREF);
	void setBackground(COLORREF);
	void setButtons(bool);
	void setFullSelect(bool);
	void setAutoExpand(bool);
	void setImageList(ImageList*);
	void select(TreeItem*);
	void appendItem(TreeItem*);
	void removeItem(TreeItem*);
	void sortChildrens();
	void clear();
	TreeItem* hitTest();
	TreeItem* selectedItem();
	size_t size();
protected:
	f_onSelectItem mOnSelectionChanged = nullptr;
	f_onClick mOnClick = nullptr;
	f_onDoubleClick mOnDoubleClick = nullptr;
	std::unordered_map<HTREEITEM, TreeItem*> mItems;

	virtual LRESULT cnotify(UINT, WPARAM, LPARAM);
};