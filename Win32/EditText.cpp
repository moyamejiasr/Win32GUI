#include "EditText.h"

EditText::EditText()
{
}

EditText::EditText(Control* parent, std::string name, RECT rect)
	: Control(parent, name, rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mBkBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mStyle |= WS_CHILD | WS_VISIBLE;
	mType = WC_EDIT;
	create();
}

EditText::EditText(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mBkBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	mStyle |= WS_CHILD | WS_VISIBLE;
	mType = WC_EDIT;
	create();
}

void EditText::replaceSelection(std::string str)
{
	if (mCreated)
		SendMessage(mHwnd, EM_REPLACESEL, TRUE, (LPARAM)str.c_str());
}

void EditText::setOnCharLimit(f_onCharLimit call)
{
	mOnCharLimit = call;
}

void EditText::setOnTextChanged(f_onTextChanged call)
{
	mOnTextChanged = call;
}

void EditText::setCharLimit(int val)
{
	mCharLimit = val;
	if (mCreated)
		SendMessage(mHwnd, EM_SETLIMITTEXT, mCharLimit, NULL);
}

void EditText::setSelection(int start, int end)
{
	// Does not draw blue
	if (mCreated)
		SendMessage(mHwnd, EM_SETSEL, start, end);
}

void EditText::setNumbersOnly(bool state)
{
	if (state)
		appendFlag(ES_NUMBER);
	else
		removeFlag(ES_NUMBER);
}

void EditText::setReadOnly(bool state)
{
	if (state)
		appendFlag(ES_READONLY);
	else
		removeFlag(ES_READONLY);

	// ES_READONLY needs special message
	// when control is already created.
	if (mCreated)
		SendMessage(mHwnd, EM_SETREADONLY, state, NULL);
}

void EditText::setPassword(bool state)
{
	if (state)
		appendFlag(ES_PASSWORD);
	else
		removeFlag(ES_PASSWORD);

	// ES_PASSWORD needs special message
	// when control is already created.
	if (mCreated)
		SendMessage(mHwnd, EM_SETPASSWORDCHAR, state ? '*' : FALSE, NULL);
}

void EditText::setTextCase(TextCase state)
{
	removeFlag(ES_UPPERCASE);
	removeFlag(ES_LOWERCASE);

	if (state == TextCase::UpperCase)
		appendFlag(ES_UPPERCASE);
	if (state == TextCase::LowerCase)
		appendFlag(ES_LOWERCASE);
}

std::string EditText::getSelection()
{
	if (!mCreated)
		return "";
	DWORD start, end;
	SendMessage(mHwnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
	return mText.substr(start, end - start);
}

void EditText::setAlign(Align margin)
{
	removeFlag(ES_LEFT);
	removeFlag(ES_CENTER);
	removeFlag(ES_RIGHT);

	if (margin == Align::Left)
		appendFlag(ES_LEFT);
	if (margin == Align::Center)
		appendFlag(ES_CENTER);
	if (margin == Align::Right)
		appendFlag(ES_RIGHT);
}

Align EditText::getAlign()
{
	if (hasFlag(ES_LEFT))
		return Align::Left;
	if (hasFlag(ES_CENTER))
		return Align::Center;
	if (hasFlag(ES_RIGHT))
		return Align::Right;

	return Align::Undefined;
}

EditText::EditText(std::string name, int width, int height)
	: Control(nullptr, name, width, height)
{
	mStyle |= WS_CHILD | WS_VISIBLE;
	mType = WC_EDIT;
}

LRESULT EditText::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case EN_CHANGE: /* Text Change, try autosize */
		mText = pullWindowText();
		if (mOnTextChanged)
			mOnTextChanged(this, mText);
		break;
	case EN_MAXTEXT:
		if (mOnCharLimit)
			mOnCharLimit(this);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT EditText::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}