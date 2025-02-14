#include "StringUtils.hpp"

std::string StringUtils::WCHARToString(const WCHAR* wstring) {
    if (!wstring)
        return {};

    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed - 1, 0);  // -1 to remove the null terminator
    WideCharToMultiByte(CP_UTF8, 0, wstring, -1, &str[0], size_needed, nullptr, nullptr);

    return str;
}

std::wstring StringUtils::ExtractFirstArgument(const std::wstring& commandLine)
{
    if (commandLine.empty())
        return L"";

    size_t startPos = 0;
    size_t argStart = std::wstring::npos;

    // Check if executable name is quoted
    if (commandLine[0] == L'"')
    {
        if (const size_t closingQuote = commandLine.find(L'"', 1); closingQuote != std::wstring::npos)
        {
            // Argument starts after the closing quote + space
            argStart = commandLine.find_first_not_of(L' ', closingQuote + 1);
        }
    }
    else
    {
        // Find the first space after the executable name
        argStart = commandLine.find(L' ');
        if (argStart != std::wstring::npos)
        {
            argStart = commandLine.find_first_not_of(L' ', argStart); // Skip spaces
        }
    }

    if (argStart != std::wstring::npos)
    {
        const size_t argEnd = commandLine.find(L' ', argStart);
        return commandLine.substr(argStart, (argEnd != std::wstring::npos) ? (argEnd - argStart) : std::wstring::npos);
    }
    return L""; // No argument found
}

std::string StringUtils::WstringToString(const std::wstring& wstring)
{
    return std::string(wstring.begin(), wstring.end());
}