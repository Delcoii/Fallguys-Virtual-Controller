#pragma once

#include <Windows.h>
#include <ViGEm/ViGEmClient.h>
#include "VigemController.hpp"

// LeftStickControl
// - W/A/S/D 키 입력을 읽어서 Xbox Left Stick 축 값으로 변환한 후
//   VigemController::SendX360Report()를 통해 가상 컨트롤러로 전송.
// - ESC 키를 누르면 제어 루프가 종료됨.
class LeftStickControl
{
public:
    // 생성자: 이미 Initialize() + RegisterX360()이 완료된 VigemController를 참조로 받음
    explicit LeftStickControl(VigemController& controller);

    // 제어 루프 실행
    // - 매 프레임마다 키 상태를 읽음
    // - BuildReportFromKeys()로 XUSB_REPORT를 만든 뒤 SendX360Report() 호출
    // - ESC 키 입력 또는 LoopExit() 호출 시 루프 종료
    void Run();

    // 외부에서 호출하면 제어 루프를 즉시 종료하도록 설정
    void LoopExit() { should_exit_ = true; }

private:
    VigemController& controller_;  // 이미 초기화된 VigemController 참조
    bool should_exit_;             // 루프 강제 종료 플래그

    // W/A/S/D 키 상태를 기반으로 XUSB_REPORT 작성
    XUSB_REPORT BuildReportFromKeys();

    bool before_press_state_up_ = false;
    bool before_press_state_down_ = false;
    bool before_press_state_left_ = false;
    bool before_press_state_right_ = false;

    
    int up_count_ = 0;
    int down_count_ = 0;
    int left_count_ = 0;
    int right_count_ = 0;

};
