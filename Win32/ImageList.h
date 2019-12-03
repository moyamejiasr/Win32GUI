#pragma once
#pragma warning (disable : 26495)

#define _AFXDLL
#include <SDKDDKVer.h>
#include <afxwin.h>

#include <string>
#include <vector>

class ImageList
{
public:
	ImageList(int, int = 32, int = 32);
	~ImageList();
	void append(HICON);
	HIMAGELIST handler();
protected:
	HIMAGELIST mIList;
	std::vector<HICON> mIcons;
};