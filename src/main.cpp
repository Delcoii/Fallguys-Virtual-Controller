#include "VigemController.hpp"
#include "LeftStickControl.hpp"
#include <iostream>
#include <thread>

// main(): ViGEm 초기화 → 컨트롤러 등록 → LeftStickControl 루프 실행 → Cleanup
int main() {
    std::string mainpage0 = " _______   _______  __        ______   ______    __   __  \n";
    std::string mainpage1 = "|       \\ |   ____||  |      /      | /  __  \\  |  | |  | \n";
    std::string mainpage2 = "|  .--.  ||  |__   |  |     |  ,----'|  |  |  | |  | |  | \n";
    std::string mainpage3 = "|  |  |  ||   __|  |  |     |  |     |  |  |  | |  | |  | \n";
    std::string mainpage4 = "|  '--'  ||  |____ |  `----.|  `----.|  `--'  | |  | |  | \n";
    std::string mainpage5 = "|_______/ |_______||_______| \\______| \\______/  |__| |__|\n\n\n";

    // Show the main page
    std::cout << mainpage0 << mainpage1 << mainpage2 << mainpage3 << mainpage4 << mainpage5;


    VigemController controller;

    // 1) ViGEm client 초기화
    if (!controller.Initialize()) {
        return -1;
    }

    // 2) 가상 Xbox360 컨트롤러 등록
    if (!controller.RegisterX360()) {
        // 등록 실패 시 즉시 Cleanup 후 종료
        controller.Cleanup();
        return -1;
    }


    // 3) 가상 컨트롤러가 완전히 생성될 때까지 잠깐 대기
    std::cout << "[Info] Waiting 1 second before starting LeftStickControl...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 4) LeftStickControl 실행
    LeftStickControl leftStick(controller);
    leftStick.Run();

    // 5) 루프 종료 후 컨트롤러 제거 및 client 해제
    controller.Cleanup();
    return 0;
}
