#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <intrin.h>
#include <winternl.h>

#include "StringUtils.hpp"

class ProcessUtils {
public:
    static void TerminateProcessByName(const std::string& processName);
    static DWORD GetProcessID(const std::wstring &processName);
    static std::string GetCallerModuleName();
    static std::wstring GetClientServiceUrl();
    static std::wstring GetProcessCommandLine(DWORD processID);
};
