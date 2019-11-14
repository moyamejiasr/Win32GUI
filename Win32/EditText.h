#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

using f_onCharLimit = void(*)(EditText*);

enum TextCase {
	Normal,
	UpperCase,
	LowerCase
};

class EditText : public TextControl
{
public:
	EditText();
	EditText(Control*, std::string, int = 0, int = 0);

	void replaceSelection(std::string);
	void setOnCharLimit(f_onCharLimit);
	void setCharLimit(int);
	void setSelection(int, int);
	void setNumbersOnly(bool);
	void setReadOnly(bool);
	void setPassword(bool);
	void setTextCase(TextCase);
	std::string getSelection();
	virtual void setMargin(Margin);
	virtual Margin getMargin();
protected:
	EditText(std::string, int = 0, int = 0);
	f_onCharLimit mOnCharLimit = nullptr;
	int mCharLimit = 0;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};