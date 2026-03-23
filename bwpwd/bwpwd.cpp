#include <windows.h>
#include <iostream>

//With the way the cd tool is built this isn't necessarily needed because cd will tell you the directory and if a move works
int main() {
    // 1. Create a buffer to hold the path
    wchar_t buffer[MAX_PATH];

    // 2. Call the Windows API to get the current working directory
    // Returns the number of characters written, or 0 on failure
    DWORD length = GetCurrentDirectoryW(MAX_PATH, buffer);

    if (length == 0) {
        std::wcerr << L"Error: Could not retrieve current directory. Code: " << GetLastError() << std::endl;
        return 1;
    }

    // 3. Print the result to the console
    std::wcout << buffer << std::endl;

    return 0;
}
