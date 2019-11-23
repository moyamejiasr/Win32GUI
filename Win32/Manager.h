#pragma once

#include "Window.h"
#ifndef _MINIMAL
#include "TextView.h"
#include "PictureBox.h"
#include "EditText.h"
#include "MultiEditText.h"
#include "Button.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "FixedComboBox.h"
#include "TrackBar.h"
#include "ProgressBar.h"
#endif
#ifdef _OGL
#include "OGLFrame.h"
#endif

#define Invisible (HBRUSH)GetStockObject(HOLLOW_BRUSH)

// Get COLORREF from r, g, b values
// Example: 255, 255, 255
COLORREF Rgb(BYTE r, BYTE g, BYTE b)
{
	return RGB(r, g, b);
}

// Get COLORREF from Hex dword
// Example: 0xFFFFFF
COLORREF Hex(DWORD h)
{
	return Rgb(BYTE(h >> 16), BYTE(h >> 8), BYTE(h));
}

// Get COLORREF from Hex string
// Example: "#FFFFFF"
COLORREF Hex(std::string hex)
{
	if (hex[0] == '#')
		hex = hex.erase(0, 1);
	return Hex(std::stoul(hex, nullptr, 16));
}

// Get Solid Brush from Color
HBRUSH Brush(COLORREF ref)
{
	return CreateSolidBrush(ref);
}

// Load Bitmap from file
HBITMAP Bitmap(std::string dir) {
	return (HBITMAP)LoadImage(NULL, dir.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

// Load Bitmap from resource
HBITMAP Bitmap(HINSTANCE hInstance, WORD id) {
	return LoadBitmap(hInstance, MAKEINTRESOURCE(id));
}

// Load Icon from file
HICON Icon(std::string dir) {
	return (HICON)LoadImage(NULL, dir.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

// Load Icon from resource
HICON Icon(HINSTANCE hInstance, WORD id) {
	return LoadIcon(hInstance, MAKEINTRESOURCE(id));
}

// Load Menu from resource
HMENU Menu(HINSTANCE hInstance, WORD id) {
	return LoadMenu(hInstance, MAKEINTRESOURCE(id));
}

int Dialog(std::string text, std::string title) {
	return MessageBox(NULL, text.c_str(), title.c_str(), MB_SYSTEMMODAL | MB_OK);
}