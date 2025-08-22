@echo off
echo Building Fall Guys Virtual Controller (Static)...

REM Backup original manifest
if exist admin.manifest (
    copy admin.manifest admin.manifest.backup
    echo Manifest backed up
)

REM Build with manifest
cl /MT /O2 /EHsc /I "vigem_client\include" ^
    /I "include" ^
    /I "ini_parser\include" ^
    "src\VigemController.cpp" ^
    "src\LeftStickControl.cpp" ^
    "src\MenuButtonControl.cpp" ^
    "ini_parser\src\ini_parser.cpp" ^
    "src\main.cpp" ^
    /link ^
    /LIBPATH:"vigem_client\lib\release" ^
    /MANIFEST ^
    /MANIFESTFILE:"admin.manifest" ^
    /OUT:"build\main.exe" ViGEmClient.lib user32.lib

REM Check if build was successful
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    if exist admin.manifest.backup (
        copy admin.manifest.backup admin.manifest
        del admin.manifest.backup
    )
    pause
    exit /B 1
)

REM Restore original manifest
if exist admin.manifest.backup (
    copy admin.manifest.backup admin.manifest
    del admin.manifest.backup
    echo Original manifest restored
)

echo Build completed successfully!
echo Executable: build\main.exe
pause