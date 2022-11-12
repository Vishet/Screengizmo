#pragma once

#include <exception>
#include <Windows.h>
#include <gdiplus.h>
#include <string>

#pragma comment(lib,"Gdiplus.lib")


#define THROW_IF_FAILED_HR(result) if(FAILED(hr = result)) throw Exception(hr, __LINE__, __FILE__) 
#define THROW_IF_LAST_FAILED(result) if(!result) throw Exception(GetLastError(), __LINE__, __FILE__)
#define THROW_IF_FAILED_GDIP(result) if(status = result) throw Exception(status, __LINE__, __FILE__)
#define THROW_IF_FAILED_GDI(result, msg) if(!result) throw Exception(msg, __LINE__, __FILE__)
#define THROW_EXCEPTION(msg) throw Exception(msg, __LINE__, __FILE__)

class Exception : std::exception
{
private:
	enum class Type
	{
		WINDOWS,
		GDI,
		GDIPLUS
	};

	const Type type;

	HRESULT hr{};
	Gdiplus::Status status{};
	mutable std::string message{};

	int line;
	std::string filePath;
	mutable std::string whatString;

public:
	Exception(HRESULT hr, int line, std::string_view filePath) noexcept;
	Exception(Gdiplus::Status status, int line, std::string_view filePath) noexcept;
	Exception(std::string_view message, int line, std::string_view filePath) noexcept;

	Exception(const Exception&) = delete;
	Exception(const Exception&&) = delete;
	Exception operator=(const Exception&) = delete;
	Exception operator=(const Exception&&) = delete;

	const char* what() const noexcept override;
	std::string GetDescription() const noexcept;
	std::string GetStatusDescription() const noexcept;
};

