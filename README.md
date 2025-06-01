# Fallguys-Virtual-Controller
가상패드 부흥을 위한, 그리고 나도 불편해서


# Prerequisites

## ViGEm BUS Installer
* https://github.com/ViGEm/ViGEmBus/releases
* ViGEmBus_1.21.442_x64_x86_arm64.exe







# 개발 참고
## Build
### 가상패드 on
```bash
cl /MD /O2 /EHsc /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\include" /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src" "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\VigemController.cpp" "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\LeftStickControl.cpp" "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\main.cpp" /link /LIBPATH:"C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\lib\release" /OUT:"build\main.exe" ViGEmClient.lib user32.lib
```

### connection test script
```bash
cl /EHsc /I "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\include"  "C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\src\test.cpp" /link /LIBPATH:"C:\Users\sungh\Desktop\git\Fallguys-Virtual-Controller\vigem_client\lib" /OUT:"build\test.exe" ViGEmClient.lib
```


## Visual Studio 2022
* https://visualstudio.microsoft.com/ko/downloads/
* 아래 Visual Studio 2022용 빌드 도구 를 설치 (https://aka.ms/vs/17/release/vs_BuildTools.exe)
* C++를 이용한 데스크톱 개발 선택하고 다운로드

## .dll, .lib 생성 시 v141? 필요 (visual studio 2017 빌드툴로 sln 실행해야돼서)

## 배포는 앞에 이걸 붙여야한다함 (O2)
```bash
cl /EHsc /MD /O2 ...
```



## .Net sdk (근데 이거 위에꺼 설치할때 해도 될 듯?)
https://dotnet.microsoft.com/en-us/download/dotnet/thank-you/sdk-2.1.818-windows-x64-installer

## ViGEm SDK (Header, Library)
https://github.com/ViGEm/ViGEmClient/releases

## 너무 설치과정 복잡해서 잘 모르겠음 이제
