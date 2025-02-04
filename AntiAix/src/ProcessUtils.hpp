#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>

class ProcessUtils {
public:
    static void TerminateProcessByName(const std::string& processName);
    static DWORD GetProcessID(const std::wstring& processName);
};
