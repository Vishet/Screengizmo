#pragma once

#include <windows.h>
#include <string>
#include <gdiplus.h>

class FileSaver
{
public:
	FileSaver(const FileSaver&) = delete;
	FileSaver(const FileSaver&&) = delete;
	FileSaver operator=(const FileSaver&) = delete;
	FileSaver operator=(const FileSaver&&) = delete;

	FileSaver() noexcept;
	~FileSaver() noexcept;

	void Save(HWND sourceWindow) const;

private:
	HBITMAP GetBackgroundHandle(HWND sourceWindow) const;
	std::wstring OpenSaveDialog() const;
	void SaveBitmapToFile(HBITMAP bitmapHandle, const std::wstring& fileName) const;
	std::wstring GetMimeType(const std::wstring& fileName) const;
	CLSID GetEncoderClsid(const std::wstring& mimeType) const;
	void SetJPEGEncoderParameters(Gdiplus::EncoderParameters** encoderParameters) const noexcept;
};

