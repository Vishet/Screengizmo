#include "Exception.h"
#include <sstream>

Exception::Exception(HRESULT hr, int line, std::string_view filePath) noexcept :
	hr{ hr },
	line{ line },
	filePath{ filePath },
	type{ Type::WINDOWS }
{
}

Exception::Exception(Gdiplus::Status status, int line, std::string_view filePath) noexcept :
	status{ status },
	line{ line },
	filePath{ filePath },
	type{ Type::GDIPLUS }
{
}

Exception::Exception(std::string_view message, int line, std::string_view filePath) noexcept :
	message{ message },
	line{ line },
	filePath{ filePath },
	type{ Type::GDI }
{
}

const char* Exception::what() const noexcept
{
	std::ostringstream oss{};
	switch (type)
	{
	case Type::WINDOWS:
		oss << "Code: 0x" << std::hex << hr << " (" << std::dec << hr << ')' << std::endl;
		message = GetDescription();
		break;
	case Type::GDI:
		oss << "GDI error, no code/description avaliable!" << std::endl;
		break;
	case Type::GDIPLUS:
		oss << "Status: " << status << std::endl;
		message = GetStatusDescription();
	}

	oss << "Line: " << line << std::endl
		<< "File: " << filePath << std::endl
		<< std::endl
		<< message << std::endl;

	whatString = oss.str();
	return whatString.c_str();
}

std::string Exception::GetDescription() const noexcept
{
	char* formatedPointer{};

	DWORD msgLength{ FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // try to put 0
		reinterpret_cast<LPSTR>(&formatedPointer),
		0,
		nullptr
	) };

	if (msgLength == 0)
	{
		return "Undentified Error Code!";
	}
	else
	{
		std::string formatedString{ formatedPointer };
		LocalFree(formatedPointer);
		return formatedString;
	}
}

std::string Exception::GetStatusDescription() const noexcept
{
	switch (status)
	{
	case Gdiplus::Ok: return "Ok";
	case Gdiplus::GenericError: return "GenericError";
	case Gdiplus::InvalidParameter: return "InvalidParameter";
	case Gdiplus::OutOfMemory: return "OutOfMemory";
	case Gdiplus::ObjectBusy: return "ObjectBusy";
	case Gdiplus::InsufficientBuffer: return "InsufficientBuffer";
	case Gdiplus::NotImplemented: return "NotImplemented";
	case Gdiplus::Win32Error: return "Win32Error";
	case Gdiplus::Aborted: return "Aborted";
	case Gdiplus::FileNotFound: return "FileNotFound";
	case Gdiplus::ValueOverflow: return "ValueOverflow";
	case Gdiplus::AccessDenied: return "AccessDenied";
	case Gdiplus::UnknownImageFormat: return "UnknownImageFormat";
	case Gdiplus::FontFamilyNotFound: return "FontFamilyNotFound";
	case Gdiplus::FontStyleNotFound: return "FontStyleNotFound";
	case Gdiplus::NotTrueTypeFont: return "NotTrueTypeFont";
	case Gdiplus::UnsupportedGdiplusVersion: return "UnsupportedGdiplusVersion";
	case Gdiplus::GdiplusNotInitialized: return "GdiplusNotInitialized";
	case Gdiplus::PropertyNotFound: return "PropertyNotFound";
	case Gdiplus::PropertyNotSupported: return "PropertyNotSupported";
	default: return "Status Type Not Found.";
	}
}