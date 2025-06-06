@echo off
cl /MD /O2 /EHsc /I "vigem_client\include" ^
    /I "include" ^
    /I "ini_parser\include" ^
    "src\VigemController.cpp" ^
    "src\LeftStickControl.cpp" ^
    "ini_parser\src\ini_parser.cpp" ^
    "src\main.cpp" ^
    /link ^
    /LIBPATH:"vigem_client\lib\release" ^
    /OUT:"build\main.exe" ViGEmClient.lib user32.lib
exit /B 0