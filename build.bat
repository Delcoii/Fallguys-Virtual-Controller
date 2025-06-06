@echo off
cl /MD /O2 /EHsc /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\include" ^
    /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src" ^
    /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\ini_parser\include" ^
    "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\VigemController.cpp" ^
    "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\LeftStickControl.cpp" ^
    "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\ini_parser\src\ini_parser.cpp" ^
    "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\main.cpp" ^
    /link ^
    /LIBPATH:"C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\lib\release" ^
    /OUT:"build\main.exe" ViGEmClient.lib user32.lib
pause