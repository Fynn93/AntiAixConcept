#include <iostream>
#include <thread>
#include <include/MinHook.h>

#include "ProcessUtils.hpp"

BOOL BlockInputDetour(BOOL Block)
{
    std::cout << "BlockInputDetour" << std::endl;
    return TRUE;
}

bool m_Running = false;
int main()
{
    MH_Initialize();
    auto status = MH_CreateHook(reinterpret_cast<void*>(&BlockInput), reinterpret_cast<void*>(&BlockInputDetour), nullptr);
    if (status != MH_OK) {
        std::cerr << "Failed to hook BlockInput" << std::endl;
        return 1;
    }
    status = MH_EnableHook(reinterpret_cast<void*>(&BlockInput));
    if (status != MH_OK) {
        std::cerr << "Failed to enable BlockInput hook" << std::endl;
        return 1;
    }
    std::cout << "Successfully hooked BlockInput!" << std::endl;
    std::cout << "Entering main loop..." << std::endl;
    while (!m_Running) {
        ProcessUtils::TerminateProcessByName("LockDesktop.exe");
        Sleep(1000);
    }
    MH_Uninitialize();
    return 0;
}