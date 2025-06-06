# Fallguys-Virtual-Controller
* 가상패드 부흥을 위한, 그리고 나도 불편해서
* I used this project -> https://github.com/ViGEm/ViGEmBus (Thank you!)


# Prerequisites
## ViGEm BUS Installer
* https://github.com/ViGEm/ViGEmBus/releases
* ViGEmBus_1.21.442_x64_x86_arm64.exe

## ViGEm SDK (Library for delelopers)
* https://github.com/ViGEm/ViGEmClient/releases
* Need to build by source or make dll & lib by Visual Studio (2017 or higher)




# Build & Use
## Virtual Pad
```bash
build.bat 
```

## connection test script
```bash
test_exe.bat
```

## automove (in develop)
```bash
...
```

# 개발 참고
## Visual Studio 설치 시..
* https://visualstudio.microsoft.com/ko/downloads/
* 아래 Visual Studio 2022용 빌드 도구 를 설치 (https://aka.ms/vs/17/release/vs_BuildTools.exe)
* C++를 이용한 데스크톱 개발 선택하고 다운로드
* v141 도 포함해서 설치, ViGEm SDK 내부의 sln 열고 빌드 (배포용은 release로 빌드)