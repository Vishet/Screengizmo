#include "FileSaver.h"
#include "Exception.h"

#include <ShObjIdl.h>
#include <atlbase.h>
#include <string>
#include <gdiplus.h>
#include <memory>

FileSaver::FileSaver() noexcept
{
	
}

FileSaver::~FileSaver() noexcept
{
	
}

void FileSaver::Save(HWND sourceWindow) const
{
	HBITMAP screenBitmap{ GetBackgroundHandle(sourceWindow) };
	std::wstring fileName{ OpenSaveDialog() };
	SaveBitmapToFile(screenBitmap, fileName);

	DeleteObject(screenBitmap);
}

HBITMAP FileSaver::GetBackgroundHandle(HWND sourceWindow) const
{
	HDC windowDC{ GetDC(sourceWindow) };
	THROW_IF_FAILED_GDI(windowDC, "Could not get source window device context");

	HDC memoryDC{ CreateCompatibleDC(windowDC) };
	THROW_IF_FAILED_GDI(memoryDC, "Could not create a device context compatible with the source window");

	RECT windowRect{};
	THROW_IF_LAST_FAILED(GetClientRect(sourceWindow, &windowRect));

	int windowWidth{ windowRect.right - windowRect.left };
	int windowHeight{ windowRect.bottom - windowRect.top };

	HBITMAP bitmapHandle{ CreateCompatibleBitmap(windowDC, windowWidth, windowHeight)};
	THROW_IF_FAILED_GDI(bitmapHandle, "Could not create a bitmap compatible with source window");

	THROW_IF_FAILED_GDI(SelectObject(memoryDC, bitmapHandle), "Could not select the source window bitmap into a memory DC");

	THROW_IF_FAILED_GDI(
		BitBlt(memoryDC, 0, 0, windowWidth, windowHeight, windowDC, 0, 0, SRCCOPY), 
		"Could not copy window background into a memory device context"
	);

	DeleteDC(memoryDC);
	ReleaseDC(sourceWindow, windowDC);

	return bitmapHandle;
}

std::wstring FileSaver::OpenSaveDialog() const
{
	HRESULT hr{};
	
	CComPtr<IFileSaveDialog> dialog{};
	THROW_IF_FAILED_HR(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&dialog)));

	COMDLG_FILTERSPEC filterSpec[] =
	{
		{ L"PNG", L"*.png" },
		{ L"BMP", L"*.bmp" },
		{ L"JPEG", L"*.jpg;*.jpeg" }
	};

	THROW_IF_FAILED_HR(dialog->SetFileTypes(3, filterSpec));
	THROW_IF_FAILED_HR(dialog->SetDefaultExtension(L"png"));
	THROW_IF_FAILED_HR(dialog->SetFileName(L"screenshot.png"));
	THROW_IF_FAILED_HR(dialog->Show(NULL));

	CComPtr<IShellItem> item{};
	THROW_IF_FAILED_HR(dialog->GetResult(&item));

	wchar_t* fileName{};
	THROW_IF_FAILED_HR(item->GetDisplayName(SIGDN_FILESYSPATH, &fileName));
	CoTaskMemFree(fileName);

	return std::wstring(fileName);
}

void FileSaver::SaveBitmapToFile(HBITMAP bitmapHandle, const std::wstring& fileName) const
{
	Gdiplus::Status status{};

	Gdiplus::Bitmap bitmap{bitmapHandle, NULL};
	THROW_IF_FAILED_GDIP(bitmap.GetLastStatus());

	std::wstring mimeType{ GetMimeType(fileName) };
	CLSID encoderClsid{ GetEncoderClsid(mimeType) };

	Gdiplus::EncoderParameters* encoderParameters{};
	if (mimeType.compare(L"image/jpeg") == 0)
		SetJPEGEncoderParameters(&encoderParameters);

	THROW_IF_FAILED_GDIP( bitmap.Save(fileName.c_str(), &encoderClsid, encoderParameters) );

	if (encoderParameters)
		delete encoderParameters;
}

std::wstring FileSaver::GetMimeType(const std::wstring& fileName) const
{
	size_t index{ fileName.find_first_of('.') };
	if (index == fileName.npos)
		return L"image/bmp";
	else
	{
		std::wstring fileType{ fileName.substr(index + 1) };

		if (fileType.compare(L"jpg") == 0)
			fileType = L"jpeg";

		return L"image/" + fileType;
	}
}

CLSID FileSaver::GetEncoderClsid(const std::wstring& mimeType) const
{
	Gdiplus::Status status{};

	UINT num{ 0 };
	UINT size{ 0 };
	THROW_IF_FAILED_GDIP(Gdiplus::GetImageEncodersSize(&num, &size));

	auto codecInfo{ new Gdiplus::ImageCodecInfo[size / sizeof(Gdiplus::ImageCodecInfo)] };
	THROW_IF_FAILED_GDIP(Gdiplus::GetImageEncoders(num, size, codecInfo));
	
	for (size_t i{ 0 }; i < num; i++)
	{	
		if (mimeType.compare(codecInfo[i].MimeType) == 0)
		{
			CLSID encoderClsid{ codecInfo[i].Clsid };
			delete[] codecInfo;
			return encoderClsid;
		}
	}

	delete[] codecInfo;
	throw Exception("GetEncoderClsid() could not find a CLSID with the specified mimeType", __LINE__, __FILE__);
}

void FileSaver::SetJPEGEncoderParameters(Gdiplus::EncoderParameters** ppEncoderParameters) const noexcept
{
	Gdiplus::EncoderParameters* encoderParameters{ new Gdiplus::EncoderParameters() };
	encoderParameters->Count = 1;
	encoderParameters->Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters->Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters->Parameter[0].NumberOfValues = 1;
	unsigned long quality{ 100 };
	encoderParameters->Parameter[0].Value = &quality;

	*ppEncoderParameters = encoderParameters;
}








