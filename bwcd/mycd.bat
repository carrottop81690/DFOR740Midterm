@echo off
:: %~dp0 is a special variable for "the directory containing this script"
:: We wrap it in quotes in case your Visual Studio path has spaces

"%~dp0bwcd.exe" %*

:: If the C++ code succeeded, move the actual terminal
if %errorlevel% equ 0 (
    cd /d %*
)


