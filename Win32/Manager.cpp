#include "Manager.h"

std::wstring ToWString(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ToString(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

COLORREF Rgb(BYTE r, BYTE g, BYTE b)
{
	return RGB(r, g, b);
}

COLORREF Hex(DWORD h)
{
	return Rgb(BYTE(h >> 16), BYTE(h >> 8), BYTE(h));
}

COLORREF Hex(std::string hex)
{
	if (hex[0] == '#')
		hex = hex.erase(0, 1);
	return Hex(std::stoul(hex, nullptr, 16));
}

HBRUSH Brush(COLORREF ref)
{
	return CreateSolidBrush(ref);
}

HBITMAP Bitmap(std::string& dir)
{
	return (HBITMAP)LoadImage(NULL, dir.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

HBITMAP Bitmap(WORD id)
{
	return LoadBitmap(Control::hinstance(), MAKEINTRESOURCE(id));
}

HBITMAP Bitmap(HINSTANCE hInstance, WORD id)
{
	return LoadBitmap(hInstance, MAKEINTRESOURCE(id));
}

HICON Icon(std::string& dir)
{
	return (HICON)LoadImage(NULL, dir.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

HICON Icon(WORD id)
{
	return LoadIcon(Control::hinstance(), MAKEINTRESOURCE(id));
}

HICON Icon(HINSTANCE hInstance, WORD id)
{
	return LoadIcon(hInstance, MAKEINTRESOURCE(id));
}

HMENU Menu(WORD id)
{
	return LoadMenu(Control::hinstance(), MAKEINTRESOURCE(id));
}

HMENU Menu(HINSTANCE hInstance, WORD id)
{
	return LoadMenu(hInstance, MAKEINTRESOURCE(id));
}

int Dialog(std::string text, std::string title, DWORD flags)
{
	return MessageBox(NULL, text.c_str(), title.c_str(), flags);
}

std::string OpenFileDialog(UINT size, const COMDLG_FILTERSPEC* c_rgFileTypes)
{
	std::string path;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return path;

	// Create the FileOpenDialog object.
	IFileOpenDialog* pFileOpen;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)(&pFileOpen));
	hr = pFileOpen->SetOptions(FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);
	hr = pFileOpen->SetFileTypes(size, c_rgFileTypes);

	// Show the Open dialog box.
	if (FAILED(hr) || FAILED(pFileOpen->Show(NULL)))
	{
		CoUninitialize();
		return path;
	}

	// Get the file name from the dialog box.
	IShellItem* pItem;
	if (SUCCEEDED(pFileOpen->GetResult(&pItem)))
	{
		LPWSTR pszFilePath;
		if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
		{
			path = ToString(pszFilePath);
			CoTaskMemFree(pszFilePath);
		}
		pItem->Release();
	}

	pFileOpen->Release();
	CoUninitialize();
	return path;
}

std::vector<std::string> OpenMultiFileDialog(UINT size, const COMDLG_FILTERSPEC* c_rgFileTypes)
{
	std::vector<std::string> path;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return path;

	// Create the FileOpenDialog object.
	IFileOpenDialog* pFileOpen;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)(&pFileOpen));
	hr = pFileOpen->SetOptions(FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT | FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST);
	hr = pFileOpen->SetFileTypes(size, c_rgFileTypes);

	// Show the Open dialog box.
	if (FAILED(hr) || FAILED(pFileOpen->Show(NULL)))
	{
		CoUninitialize();
		return path;
	}

	// Get the file name from the dialog box.
	IShellItemArray* pItems;
	if (SUCCEEDED(pFileOpen->GetResults(&pItems)))
	{
		DWORD dwNumItems = 0; // number of items in multiple selection
		hr = pItems->GetCount(&dwNumItems);  // get number of selected items

		// Loop through IShellItemArray and construct string for display
		for (DWORD i = 0; i < dwNumItems; i++)
		{
			IShellItem* pItem = NULL;
			if (SUCCEEDED(pItems->GetItemAt(i, &pItem)))
			{
				LPWSTR pszFilePath;
				if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
				{
					path.push_back(ToString(pszFilePath));
					CoTaskMemFree(pszFilePath);
				}
				pItem->Release();
			}
		}
		pItems->Release();
	}

	pFileOpen->Release();
	CoUninitialize();
	return path;
}

std::string SaveFileDialog(UINT size, const COMDLG_FILTERSPEC* c_rgFileTypes, std::string fName)
{
	std::string path;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return path;

	// Create the FileOpenDialog object.
	IFileSaveDialog* pFileSave;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, (void**)(&pFileSave));
	hr = pFileSave->SetOptions(FOS_FORCEFILESYSTEM | FOS_OVERWRITEPROMPT);
	hr = pFileSave->SetFileTypes(size, c_rgFileTypes);
	hr = pFileSave->SetFileName(ToWString(fName).c_str());
	if (size) // Append the extension to the file to create
		hr = pFileSave->SetDefaultExtension(c_rgFileTypes[0].pszSpec);

	// Show the Open dialog box.
	if (FAILED(hr) || FAILED(pFileSave->Show(NULL)))
	{
		CoUninitialize();
		return path;
	}

	// Get the file name from the dialog box.
	IShellItem* pItem;
	if (SUCCEEDED(pFileSave->GetResult(&pItem)))
	{
		LPWSTR pszFilePath;
		if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
		{
			path = ToString(pszFilePath);
			CoTaskMemFree(pszFilePath);
		}
		pItem->Release();
	}

	pFileSave->Release();
	CoUninitialize();
	return path;
}

std::string SelectFolderDialog()
{
	std::string path;
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		return path;

	// Create the FileOpenDialog object.
	IFileOpenDialog* pFolderSave;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)(&pFolderSave));
	hr = pFolderSave->SetOptions(FOS_FORCEFILESYSTEM | FOS_PICKFOLDERS);

	// Show the Open dialog box.
	if (FAILED(hr) || FAILED(pFolderSave->Show(NULL)))
	{
		CoUninitialize();
		return path;
	}

	// Get the file name from the dialog box.
	IShellItem* pItem;
	if (SUCCEEDED(pFolderSave->GetResult(&pItem)))
	{
		LPWSTR pszFilePath;
		if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
		{
			path = ToString(pszFilePath);
			CoTaskMemFree(pszFilePath);
		}
		pItem->Release();
	}

	pFolderSave->Release();
	CoUninitialize();
	return path;
}
