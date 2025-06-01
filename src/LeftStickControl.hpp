#pragma once

#include <Windows.h>
#include <ViGEm/ViGEmClient.h>
#include "VigemController.hpp"

namespace MOVING_TYPE {
    enum Type {
        NORMAL = 0,  // Normal mode -> left+right command might stop jelly moving
        FAST = 1     // Fast mode   -> left+right command will move jelly of last input
    };
}

// LeftStickControl
// wasd or tfgh to move, space to jump
// f12 to exit, pgup or pgdn to switch mode (normal/fast)
class LeftStickControl {
public:
 
    explicit LeftStickControl(VigemController& controller);

    void Run();

    void LoopExit() { should_exit_ = true; }

private:
    VigemController& controller_;  
    bool should_exit_;

    // calculate command report from key states
    XUSB_REPORT BuildReportFromKeys(const int moving_mode);

    bool before_press_state_up_ = false;
    bool before_press_state_down_ = false;
    bool before_press_state_left_ = false;
    bool before_press_state_right_ = false;
    
    int up_count_ = 0;
    int down_count_ = 0;
    int left_count_ = 0;
    int right_count_ = 0;

    int moving_mode_ = MOVING_TYPE::FAST; // Default mode is FAST

};
