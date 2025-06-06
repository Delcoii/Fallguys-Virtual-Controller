#pragma once

#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>

#include <ViGEm/ViGEmClient.h>
#include "VigemController.hpp"

#include "KeyTable.hpp"        // Key mapping table
#include "ini_parser.h"


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
    void AutoMoveJelly();

    void LoopExit() { should_exit_ = true; }

private:
    // Get input key from ini file
    void ProcessINI();

    // calculate command report from key states
    XUSB_REPORT BuildReportFromKeys(const int moving_mode);
    
    // Controller
    VigemController& controller_;  
    bool should_exit_;
    bool signal_on_ = true;  // signal on/off state

    // Key Settings configuration
    IniParser ini_parser_;
    int up_key_;
    int down_key_;
    int left_key_;
    int right_key_;
    int jump_key_;
    int toggle_mode_key_;
    int controller_onoff_key_;

    // loop period detection
    std::chrono::steady_clock::time_point loop_start_time_ = std::chrono::steady_clock::now();
    std::chrono::microseconds last_period_us_;

    // Default mode is FAST
    int moving_mode_ = MOVING_TYPE::FAST; 

    // variables for calculating Left Stick command
    bool before_press_state_up_ = false;
    bool before_press_state_down_ = false;
    bool before_press_state_left_ = false;
    bool before_press_state_right_ = false;
    
    int up_count_ = 0;
    int down_count_ = 0;
    int left_count_ = 0;
    int right_count_ = 0;

    int before_stick_pos_x_ = 0;
    int before_stick_pos_y_ = 0;
};
