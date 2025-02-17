#include "ProcessUtils.hpp"

void ProcessUtils::TerminateProcessByName(const std::string& processName) {
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system
    const HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return;
    }

    // Set the size of the structure before using it
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to retrieve process information. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcessSnap); // Clean the snapshot object
        return;
    }

    // Walk the snapshot of processes
    do {
        // Compare the process name with the target name
        if (std::string currentProcessName(pe32.szExeFile); currentProcessName != processName) continue;
        // Open the process
        const HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
        if (hProcess == nullptr) {
            std::cerr << "Failed to open process " << processName << " (PID: " << pe32.th32ProcessID << "). Error: " << GetLastError() << std::endl;
            continue;
        }

        // Terminate the process
        if (TerminateProcess(hProcess, 0)) {
            std::cout << "Successfully terminated process " << processName << " (PID: " << pe32.th32ProcessID << ")" << std::endl;
        } else {
            std::cerr << "Failed to terminate process " << processName << " (PID: " << pe32.th32ProcessID << "). Error: " << GetLastError() << std::endl;
        }

        // Close the process handle
        CloseHandle(hProcess);
    } while (Process32Next(hProcessSnap, &pe32)); // Move to the next process

    // Clean up the snapshot object
    CloseHandle(hProcessSnap);
}

DWORD ProcessUtils::GetProcessID(const std::wstring& processName) {
    DWORD processID = 0;
    const HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    // Iterate through the processes
    if (Process32FirstW(hSnap, &pe32)) {
        do {
            // Compare the process name with the target name
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                processID = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnap, &pe32));
    }
    CloseHandle(hSnap);
    return processID;
}

// TODO: Remove this? Maybe use it for dll injection
std::string ProcessUtils::GetCallerModuleName()
{
    const PVOID returnAddress = _ReturnAddress();

    // Get module handle from the return address
    HMODULE hModule = nullptr;
    GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPCWSTR>(returnAddress), &hModule);

    if (hModule)
    {
        if (WCHAR moduleName[MAX_PATH]; GetModuleFileNameW(hModule, moduleName, MAX_PATH))
        {
            return StringUtils::WCHARToString(moduleName);
        }
    }
    return "Unknown Module";
}

std::wstring ProcessUtils::GetClientServiceUrl()
{
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system
    const HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create process snapshot. Error: " << GetLastError() << std::endl;
        return L"";
    }

    // Set the size of the structure before using it
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to retrieve process information. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcessSnap); // Clean the snapshot object
        return L"";
    }

    // Walk the snapshot of processes
    do {
        // Compare the process name with the target name
        if (std::string currentProcessName(pe32.szExeFile); currentProcessName != "UZTrayApp.exe")
            continue;
        if (std::wstring cmdLine = GetProcessCommandLine(pe32.th32ProcessID); !cmdLine.empty()) {
            if (std::wstring firstArg = StringUtils::ExtractFirstArgument(cmdLine); !firstArg.empty()) {
                if (!firstArg.ends_with(L"/")) {
                    firstArg += L"/";
                }
                return firstArg;
            }
        }
    } while (Process32Next(hProcessSnap, &pe32)); // Move to the next process

    // Clean up the snapshot object
    CloseHandle(hProcessSnap);
    return L""; // No client service URL found
}

std::wstring ProcessUtils::GetProcessCommandLine(const DWORD processID) {
    std::wstring commandLine;
    const HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (!hProcess) return L"";

    PROCESS_BASIC_INFORMATION pbi;
    ULONG len;
    if (const NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len); status != 0) {
        CloseHandle(hProcess);
        return L"";
    }

    PEB peb;
    if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), nullptr)) {
        CloseHandle(hProcess);
        return L"";
    }

    RTL_USER_PROCESS_PARAMETERS params;
    if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &params, sizeof(params), nullptr)) {
        CloseHandle(hProcess);
        return L"";
    }

    WCHAR buffer[32768]; // Large buffer for command line
    if (!ReadProcessMemory(hProcess, params.CommandLine.Buffer, buffer, params.CommandLine.Length, nullptr)) {
        CloseHandle(hProcess);
        return L"";
    }

    commandLine.assign(buffer, params.CommandLine.Length / sizeof(WCHAR));
    CloseHandle(hProcess);
    return commandLine;
}