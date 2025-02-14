#pragma once

#include <string>
#include <windows.h>

class StringUtils {
public:
    static std::string WCHARToString(const WCHAR* wstring);
    static std::wstring ExtractFirstArgument(const std::wstring& commandLine);
    static std::string WstringToString(const std::wstring& wstring);
};
