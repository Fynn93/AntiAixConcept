#include "AixUtils.hpp"

int AixUtils::GetServerServicePort() {
    int port = 8001; // Default port
    HKEY hKey;

    // Try first registry path
    if (const auto regPath1 = L"SOFTWARE\\AixConcept GmbH\\UZ";
        RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath1, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        // Try the second registry path if the first one fails
        if (const auto regPath2 = L"SOFTWARE\\Wow6432Node\\AixConcept GmbH\\UZ";
            RegOpenKeyExW(HKEY_LOCAL_MACHINE, regPath2, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            return port; // If both fail, return default
        }
    }

    DWORD data;
    DWORD dataSize = sizeof(data);
    DWORD dataType;

    // Query the "ServerServicePort" value
    if (RegQueryValueExW(hKey, L"ServerServicePort", nullptr, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize) == ERROR_SUCCESS) {
        if (dataType == REG_DWORD) {
            port = static_cast<int>(data);
        }
    }

    RegCloseKey(hKey);
    return port;
}