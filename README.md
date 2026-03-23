# Windows API CLI Tools (bwdir, bwpwd, bwcd)

A suite of custom Windows command-line utilities written in **C++** utilizing the **Win32 API**. These tools replicate and enhance core system functionality using low-level system calls for file enumeration, directory management, and security descriptor retrieval.

## 🛠 Educational Disclaimer
This project was developed strictly for **educational purposes** as part of an academic assignment. It serves as a practical exploration of the Windows API, process management, and the security implications of command-line reconnaissance tools.
## 🚀 Getting Started

### Prerequisites
*   **Windows 10/11**
*   **Visual Studio 2019/2022** with the **"Desktop development with C++"** workload installed.

### Installation & Setup
1.  **Clone or Download**: Download the source code to your local machine.
2.  **Open Solution**: Open the `.sln` file in Visual Studio.
3.  **Build**: 
    *   Set the **Solution Configuration** to `Debug` or `Release`.
    *   Set the **Solution Platform** to `x64`.
    *   Go to **Build > Rebuild Solution**. This will compile all three projects (`bwdir`, `bwpwd`, and `bwcd`) simultaneously.
4.  **Locate Binaries**: The compiled `.exe` files and the `.bat` wrapper will be generated in your solution's output directory (typically `x64\Debug\` or `x64\Release\`).

## 🚀 Usage & Examples

### 1. `bwdir` (Enhanced Directory Listing)
A replacement for the standard `dir` command with support for ownership and recursion.

*   **Show Owners (`/q`):** Displays the user or group that owns each file.
```cmd
C:\Users\User> "C:\Path\To\bwdir.exe" /q
```
> **Output:**
> ```text
> 03/23/2026  09:15 AM      <DIR>          DESKTOP-PC\User           Documents
> 03/23/2026  11:42 AM             15,240  DESKTOP-PC\Admin          report.docx
> 03/22/2026  04:10 PM      <DIR>          NT AUTHORITY\SYSTEM       Config
> ```

*   **Show Hidden & System Files (`/a`):** Includes protected files that are usually hidden.
```cmd
C:\Users\User> "C:\Path\To\bwdir.exe" /a
```
> **Output:**
> ```text
> 01/15/2026  11:00 PM      <DIR>          SYSTEM                    AppData
> 02/10/2026  08:20 AM             1,024   DESKTOP-PC\User           .gitconfig
> 03/23/2026  01:05 PM               450   DESKTOP-PC\User           desktop.ini
> ```

*   **Recursive Search (`/s`):** Crawls through every subfolder and prints localized headers.
```cmd
C:\Projects> "C:\Path\To\bwdir.exe" /s
```
> **Output:**
> ```text
>  Directory of C:\Projects
> 03/23/2026  10:15 AM      <DIR>          Source
> 
>  Directory of C:\Projects\Source
> 03/23/2026  10:16 AM             5,400   main.cpp
> 03/23/2026  10:16 AM             1,200   utils.h
> ```

---

### 2. `bwpwd` (Print Working Directory)
Displays the absolute path of the current active directory.

```cmd
C:\Users\User\Documents\Source\Repos> "C:\Path\To\bwpwd.exe"
```
> **Output:** `C:\Users\User\Documents\Source\Repos`

---

### 3. `mycd.bat` (Change Directory Wrapper)
Because a child process cannot modify a parent terminal's environment, this tool uses a C`` validator (`bwcd.exe`) and a Batch wrapper (`mycd.bat`) to successfully move the prompt.

*   **Subdirectory Name:** Move into a folder within your current location.
```cmd
C:\Users\User> "C:\Path\To\mycd.bat" "Visual Studio Projects"
```
> **Output:**
>```text
>Success! Internal directory changed to: C:\Users\User\Visual Studio Projects
>C:\Users\User\Visual Studio Projects>
>```

*   **Parent Directory (`..`):** Move back one level to the parent directory.
```cmd
C:\Windows\System32> "C:\Path\To\mycd.bat" ..
```
> **Output:** 
>```text
>Success! Internal directory changed to: C:\Windows
>C:\Windows>
>```

*   **Full Absolute Path:** Jump directly to any location on the system.
```cmd
C:\Users\User> "C:\Path\To\mycd.bat" C:\Windows\System32
```
> **Output:** 
>```text
>Success! Internal directory changed to: C:\Windows\System32
>C:\Windows\System32>
>```

---

## 📖 Component Logic

*   **`bwdir.exe`**: Uses `FindFirstFileW` and `GetNamedSecurityInfoW` to list files and their NTFS owners.
*   **`bwpwd.exe`**: Uses `GetCurrentDirectoryW` to retrieve the active process path.
*   **`bwcd.exe`**: Reconstructs arguments to handle spaces and validates paths via `SetCurrentDirectoryW`.
*   **`mycd.bat`**: Uses `%~dp0` to locate the binary and triggers `cd /d` upon a successful exit code (0) from the C++ validator.

## 📂 Project Structure
The solution is organized into three distinct projects:
*   **`bwdir`**: C++ source for the enhanced directory listing tool.
*   **`bwpwd`**: C++ source for the path printing tool.
*   **`bwcd`**: C++ source for path validation and the `mycd.bat` wrapper.
## ⚖️ License
This project is released under the **Unlicense**. You are free to copy, modify, publish, use, compile, sell, or distribute this software for any purpose. For more information, please refer to <http://unlicense.org>

## 📧 Contact
**Brett Wright**  
**Email:** carrottop81690@gmail.com  
**https://github.com/carrottop81690**


