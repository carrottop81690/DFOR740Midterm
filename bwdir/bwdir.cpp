#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <aclapi.h>

#pragma comment(lib, "advapi32.lib")

unsigned __int64 totalBytes = 0;
int fileCount = 0;
int dirCount = 0;

// Formatting Helpers so it looks similar to the actual dir command
std::wstring FormatNumber(unsigned __int64 value) {
    std::wstring s = std::to_wstring(value);
    NUMBERFMTW fmt = { 0, 0, 3, (LPWSTR)L".", (LPWSTR)L",", 1 };
    wchar_t buffer[64];
    if (GetNumberFormatW(LOCALE_USER_DEFAULT, 0, s.c_str(), &fmt, buffer, 64) > 0) return buffer;
    return s;
}

std::wstring GetFileOwner(const std::wstring& path) {
    PSECURITY_DESCRIPTOR pSD = NULL;
    PSID pOwnerSid = NULL;
    if (GetNamedSecurityInfoW(path.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pOwnerSid, NULL, NULL, NULL, &pSD) != ERROR_SUCCESS) return L"...";
    wchar_t name[256], domain[256];
    DWORD nLen = 256, dLen = 256;
    SID_NAME_USE use;
    std::wstring res = L"Unknown";
    if (LookupAccountSidW(NULL, pOwnerSid, name, &nLen, domain, &dLen, &use)) res = std::wstring(domain) + L"\\" + std::wstring(name);
    LocalFree(pSD);
    return res;
}

std::wstring FormatTimestamp(FILETIME ft) {
    SYSTEMTIME st;
    FILETIME lft;
    FileTimeToLocalFileTime(&ft, &lft);
    FileTimeToSystemTime(&lft, &st);
    wchar_t d[64], t[64];
    GetDateFormatW(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, d, 64);
    GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, t, 64);
    return std::wstring(d) + L"  " + std::wstring(t);
}

// Core Logic
void RunDirRecursive(std::wstring currentPath, bool recursive, bool all, bool showOwner) {
    WIN32_FIND_DATAW fd;
    std::wstring searchPath = currentPath + L"\\*";
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    std::wcout << L"\n Directory of " << currentPath << std::endl << std::endl;
    std::vector<std::wstring> subDirs;

    do {
        std::wstring name = fd.cFileName;
        if (name == L"." || name == L"..") continue;

        bool isHidden = (fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) || (fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM);
        if (isHidden && !all) continue;

        std::wcout << FormatTimestamp(fd.ftLastWriteTime) << L"  ";
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::wcout << std::left << std::setw(15) << L"<DIR>";
            dirCount++;
            if (recursive && !(fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) subDirs.push_back(currentPath + L"\\" + name);
        }
        else {
            unsigned __int64 size = ((unsigned __int64)fd.nFileSizeHigh << 32) | fd.nFileSizeLow;
            std::wcout << std::right << std::setw(15) << FormatNumber(size);
            fileCount++;
            totalBytes += size;
        }

        if (showOwner) std::wcout << L" " << std::left << std::setw(25) << GetFileOwner(currentPath + L"\\" + name);
        std::wcout << L" " << name << std::endl;
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
    if (recursive) {
        for (size_t i = 0; i < subDirs.size(); ++i) RunDirRecursive(subDirs[i], recursive, all, showOwner);
    }
}

// Help Menu Function
void PrintHelp() {
    std::wcout << L"Displays a list of files and subdirectories in a directory.\n\n"
        << L"DIR [/A] [/Q] [/S] [/?]\n\n"
        << L"  /A          Displays files with hidden and system attributes.\n"
        << L"  /Q          Display the owner of the file.\n"
        << L"  /S          Displays files in specified directory and all subdirectories.\n"
        << L"  /?          Displays this help message.\n" << std::endl;
}

int main(int argc, char* argv[]) {
    bool recursive = false, showAll = false, showOwner = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        for (auto& c : arg) c = tolower(c); // Make case-insensitive

        if (arg == "/?" || arg == "-?" || arg == "/help") { PrintHelp(); return 0; }
        if (arg == "/s" || arg == "-s") recursive = true;
        if (arg == "/a" || arg == "-a") showAll = true;
        if (arg == "/q" || arg == "-q") showOwner = true;
    }

    wchar_t path[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, path) == 0) return 1;

    RunDirRecursive(path, recursive, showAll, showOwner);

    ULARGE_INTEGER free;
    if (GetDiskFreeSpaceExW(path, &free, NULL, NULL)) {
        std::wcout << L"\n     Total Files Listed:\n"
            << std::right << std::setw(16) << fileCount << L" File(s) " << std::setw(18) << FormatNumber(totalBytes) << L" bytes\n"
            << std::right << std::setw(16) << dirCount << L" Dir(s)  " << std::setw(18) << FormatNumber(free.QuadPart) << L" bytes free" << std::endl;
    }

    if (argc == 1) { std::wcout << L"\nDirectory request complete, press Enter to exit . . ."; std::cin.get(); }
    return 0;
}


