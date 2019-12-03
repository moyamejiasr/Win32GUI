#include "ImageList.h"

ImageList::ImageList(int size, int x, int y)
{
	mIList = ImageList_Create(x, y, ILC_COLOR32, size, 0);
}

ImageList::~ImageList()
{
	for (auto const& icon : mIcons)
		DestroyIcon(icon);
	ImageList_Destroy(mIList);
}

void ImageList::append(HICON i)
{
	mIcons.push_back(i);
	ImageList_AddIcon(mIList, i);
}

HIMAGELIST ImageList::handler()
{
	return mIList;
}
