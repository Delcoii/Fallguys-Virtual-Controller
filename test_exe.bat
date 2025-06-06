@echo off
cl /EHsc /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\include" ^
    "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\test.cpp" ^
    /link ^
    /LIBPATH:"C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\lib\release" ^
    /OUT:"build\test.exe" ViGEmClient.lib
pause