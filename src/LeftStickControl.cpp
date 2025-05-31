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

    std::cout << "[Info] LeftStickControl started (T=Up, G=Down, H=Right, F=Left). Press F12 to exit.\n";

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

    // bool t = (GetAsyncKeyState('T') & 0x8000) != 0;
    // bool f = (GetAsyncKeyState('F') & 0x8000) != 0;
    // bool g = (GetAsyncKeyState('G') & 0x8000) != 0;
    // bool h = (GetAsyncKeyState('H') & 0x8000) != 0;
    bool press_up = (GetAsyncKeyState('T') & 0x8000) != 0;
    bool press_left = (GetAsyncKeyState('F') & 0x8000) != 0;
    bool press_down = (GetAsyncKeyState('G') & 0x8000) != 0;
    bool press_right = (GetAsyncKeyState('H') & 0x8000) != 0;

    SHORT x = 0;
    SHORT y = 0;

    // Vertical axis (Y)
    if (press_up && !press_down) {
        y = FULL;      // Up
    }
    else if (press_down && !press_up) {
        y = NEG_FULL;  // Down
    }

    // Horizontal axis (X)
    if (press_right && !press_left) {
        x = FULL;      // Right
    }
    else if (press_left && !press_right) {
        x = NEG_FULL;  // Left
    }

    // Assign the stick values.
    report.sThumbLX = x;
    report.sThumbLY = y;

    static unsigned int report_count = 0;
    if (report_count++ % 5 == 0) { // Print every 100 reports
        std::cout << "[Info] Left Stick Report: X=" << x << ", Y=" << y << std::endl;
    }
    
    // Keep triggers and right stick at zero (unused).
    report.bLeftTrigger  = 0;
    report.bRightTrigger = 0;
    report.sThumbRX      = 0;
    report.sThumbRY      = 0;

    return report;
}
