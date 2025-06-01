#include "LeftStickControl.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Constructor: store the reference and initialize should_exit_ to false.
LeftStickControl::LeftStickControl(VigemController& controller)
    : controller_(controller)
    , should_exit_(false)
{
}

// Run the T/F/H/G control loop.
// Exits when F12 is pressed or should_exit_ becomes true.
void LeftStickControl::Run()
{
    // Verify that the VigemController is initialized and a controller is registered.
    if (!controller_.IsClientValid() || !controller_.IsControllerValid()) {
        std::cerr << "[Error] VigemController is not initialized or controller not registered.\n";
        return;
    }

    std::cout << "[Info] LeftStickControl started. Press F12 to exit.\n";

    // Loop until F12 is pressed or should_exit_ is true.
    while (true) {
        // If F12 is pressed, break out.
        if ((GetAsyncKeyState(VK_F12) & 0x8000) != 0) {
            std::cout << "[Info] F12 detected. Exiting LeftStickControl.\n";
            break;
        }

        // If an external request to exit was made, break out.
        if (should_exit_) {
            std::cout << "[Info] LoopExit() called. Exiting LeftStickControl.\n";
            break;
        }

        // Build and send the current Left Stick report based on T/F/H/G.
        XUSB_REPORT report = BuildReportFromKeys();
        if (!controller_.SendX360Report(report)) {
            std::cerr << "[Error] Failed to send X360 report.\n";
            break;
        }

        // Wait ~20ms (about 50 updates per second).
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    // On exit, reset Left Stick to (0,0) to center it.
    XUSB_REPORT neutral = {};
    controller_.SendX360Report(neutral);
    std::cout << "[Info] LeftStickControl loop exited.\n";
}

// Build an XUSB_REPORT from current T/F/H/G key states.
// - T moves Up (+Y), G moves Down (–Y), H moves Right (+X), F moves Left (–X).
XUSB_REPORT LeftStickControl::BuildReportFromKeys()
{
    XUSB_REPORT report = {}; // All fields start at 0.

    // Xbox Left Stick range: -32768 .. +32767
    constexpr SHORT FULL     = 32767;
    constexpr SHORT NEG_FULL = -32768;

    SHORT x = 0;
    SHORT y = 0;

    bool press_up = (GetAsyncKeyState('T') & 0x8000) != 0;
    bool press_left = (GetAsyncKeyState('F') & 0x8000) != 0;
    bool press_down = (GetAsyncKeyState('G') & 0x8000) != 0;
    bool press_right = (GetAsyncKeyState('H') & 0x8000) != 0;
    // bool press_up = (GetAsyncKeyState('W') & 0x8000) != 0;
    // bool press_left = (GetAsyncKeyState('A') & 0x8000) != 0;
    // bool press_down = (GetAsyncKeyState('S') & 0x8000) != 0;
    // bool press_right = (GetAsyncKeyState('D') & 0x8000) != 0;
    bool press_space = (GetAsyncKeyState(VK_SPACE)& 0x8000) != 0;

    static int report_count = 0;

    // check initial press time
    if (press_up == true && before_press_state_up_ == false)  up_count_ = report_count;
    if (press_down == true && before_press_state_down_ == false)  down_count_ = report_count;
    if (press_left == true && before_press_state_left_ == false)  left_count_ = report_count;
    if (press_right == true && before_press_state_right_ == false)  right_count_ = report_count;

    // joystick X axis
    if (press_left == true && press_right == true) {        // pressed both left & right
        x = (left_count_ < right_count_) ? FULL : NEG_FULL; // input is last pressed key
    }
    else if (press_left == true && press_right == false) { // pressed left only
        x = NEG_FULL; // Left
    }
    else if (press_left == false && press_right == true) { // pressed right only
        x = FULL; // Right
    }

    // joystick Y axis
    if (press_up == true && press_down == true) {          // pressed both up & down
        y = (up_count_ < down_count_) ? NEG_FULL : FULL;     // input is last pressed key
    }
    else if (press_up == true && press_down == false) {     // pressed up only
        y = FULL; // Up
    }
    else if (press_up == false && press_down == true) {     // pressed down only
        y = NEG_FULL; // Down
    }


    // Assign the stick values.
    report.sThumbLX = x;
    report.sThumbLY = y;
    if (press_space) {
        report.wButtons |= XUSB_GAMEPAD_A; 
    }

    std::string output_x = (x == NEG_FULL) ? "-" : (x == FULL) ? "+" : "o";
    std::string output_y = (y == NEG_FULL) ? "-" : (y == FULL) ? "+" : "o";
    std::string jump_state = (press_space) ? "o" : "x";
    if (report_count % 5 == 0) { // Print every 100 reports
        if (x != 0 || y != 0 || press_space == true) {
            std::cout << "[Info] Stick : " << output_x << ", " << output_y << ", Jump : " << jump_state << std::endl;
        }
    }
    
    // Keep triggers and right stick at zero (unused).
    report.bLeftTrigger  = 0;
    report.bRightTrigger = 0;
    report.sThumbRX      = 0;
    report.sThumbRY      = 0;


    before_press_state_down_ = press_down;
    before_press_state_up_ = press_up;
    before_press_state_left_ = press_left;
    before_press_state_right_ = press_right;
    report_count++;
    return report;
}
