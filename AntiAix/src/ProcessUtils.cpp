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

    if (Process32FirstW(hSnap, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                processID = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnap, &pe32));
    }
    CloseHandle(hSnap);
    return processID;
}