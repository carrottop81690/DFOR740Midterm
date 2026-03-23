#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    // 1. If no arguments, just print the current location
    if (argc < 2) {
        wchar_t current[MAX_PATH];
        if (GetCurrentDirectoryW(MAX_PATH, current)) {
            std::wcout << L"Current Directory: " << current << std::endl;
        }
        return 0;
    }

    // 2. RECONSTRUCT THE PATH: Combine all arguments into one string
    // This handles paths like: C:\Program Files\Common Files
    std::string fullPath = "";
    for (int i = 1; i < argc; ++i) {
        fullPath += argv[i];
        if (i < argc - 1) fullPath += " "; // Add space back between arguments
    }

    // 3. Convert to wide string for Windows API
    std::wstring wTarget(fullPath.begin(), fullPath.end());

    // 4. Attempt to change the directory
    if (!SetCurrentDirectoryW(wTarget.c_str())) {
        std::wcerr << L"Error: Cannot find path: " << wTarget
            << L" (Code: " << GetLastError() << L")" << std::endl;
        return 1;
    }

    // 5. Verification
    wchar_t verifiedPath[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, verifiedPath)) {
        std::wcout << L"Success! Internal directory changed to:" << std::endl;
        std::wcout << verifiedPath << std::endl;
    }

    return 0;
}
