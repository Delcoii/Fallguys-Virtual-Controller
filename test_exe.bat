@echo off
cl /EHsc /I "vigem_client\include" ^
    /I "include" ^
    "src\test.cpp" ^
    /link ^
    /LIBPATH:"vigem_client\lib\release" ^
    /OUT:"build\test.exe" ViGEmClient.lib
exit /B 0