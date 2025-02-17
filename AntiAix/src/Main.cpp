#include <iostream>
#include <thread>
#include <include/MinHook.h>

#include "Globals.hpp"
#include "utils/AixUtils.hpp"
#include "utils/ProcessUtils.hpp"
#include "wrappers/ApiWrapper.hpp"

BOOL BlockInputDetour(BOOL fBlockIt)
{
    // Disable all block input calls, so that LockDesktop.exe can't lock the input
    std::cout << "BlockInputDetour" << std::endl;
    return TRUE;
}

BOOL WINAPI HandlerRoutine(const DWORD dwCtrlType);
bool m_Running = true;
int main()
{
    MH_Initialize();
    // Hook BlockInput
    auto status = MH_CreateHook(reinterpret_cast<void*>(&BlockInput), reinterpret_cast<void*>(&BlockInputDetour), nullptr);
    if (status != MH_OK) {
        std::cerr << "Failed to hook BlockInput" << std::endl;
        return 1;
    }

    // Enable the hook
    status = MH_EnableHook(reinterpret_cast<void*>(&BlockInput));
    if (status != MH_OK) {
        std::cerr << "Failed to enable BlockInput hook" << std::endl;
        return 1;
    }
    std::cout << "Successfully hooked BlockInput!" << std::endl;

    // When the user closes the console, we uninitialize the MinHook library
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);

    // Get the client service URL from the first argument of the UZTrayApp.exe process
    Globals::g_ClientServiceUrl = StringUtils::WstringToString(ProcessUtils::GetClientServiceUrl());
    if (Globals::g_ClientServiceUrl.empty()) {
        std::cerr << "Failed to get client service URL" << std::endl;
        std::cerr << "Will not be able to disable surveillance and internet filter" << std::endl;
    } else {
        // We got the client service URL, so we can disable the internet filter and surveillance
        std::cout << "Client service url: " << Globals::g_ClientServiceUrl << std::endl;

        std::cout << ApiWrapper::SetFilterInternet(false) << std::endl;
        std::cout << ApiWrapper::SetSurveillanceAllowance(false) << std::endl;
        std::cout << "Disabled surveillance and internet filter" << std::endl;
    }

    std::cout << "Entering main loop..." << std::endl;
    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (m_Running) {
        // Check if the process is running and terminate it
        ProcessUtils::TerminateProcessByName("LockDesktop.exe");
        Sleep(1000);
    }
    MH_Uninitialize();
    return 0;
}

BOOL WINAPI HandlerRoutine(const DWORD dwCtrlType)
{
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
        m_Running = false;
    }
    return true;
}