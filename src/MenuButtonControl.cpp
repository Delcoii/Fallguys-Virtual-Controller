#include "MenuButtonControl.hpp"

MenuButtonControl::MenuButtonControl() {
    // Ini init: get exe folder path.
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::string path(exePath);
    // Extract the directory from the full exe path.
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos) {
        path = path.substr(0, pos);
    }

    // Build ini file path relative to exe directory.
    std::string iniFile = path + "\\key_settings.ini";
    ini_parser_.Init(iniFile.c_str());

    ProcessINI();
    
}

MenuButtonControl::~MenuButtonControl() {}


XUSB_REPORT MenuButtonControl::SetCheckPointResetReport(bool& is_in_checkpoint_reset) {
    
    XUSB_REPORT rep;
    
    if (is_checkpoint_started_ == false) {
        is_checkpoint_started_ = true;
        checkpoint_reset_cmd_start_time_ = std::chrono::steady_clock::now();
    }

    std::chrono::milliseconds elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - checkpoint_reset_cmd_start_time_);

    // pressing 8 key inputs
    std::vector<double> intervals_ms(9);
    for (int i = 0; i < 8; ++i) {
        intervals_ms[i] = cmd_input_interval_s_ * 1000. * (i + 1);
    }
    intervals_ms[8] = intervals_ms[7] + last_keydown_time_s_ * 1000.;

    /*
    if (elapsed_time_ms.count() < intervals_ms[0]) {
        rep.wButtons = XUSB_GAMEPAD_START;
    }
    else if (elapsed_time_ms.count() < intervals_ms[1]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[2]) {
        rep.wButtons = XUSB_GAMEPAD_DPAD_DOWN;
    }
    else if (elapsed_time_ms.count() < intervals_ms[3]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[4]) {
        rep.wButtons = XUSB_GAMEPAD_DPAD_DOWN;
    }
    else if (elapsed_time_ms.count() < intervals_ms[5]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[6]) {
        rep.wButtons = XUSB_GAMEPAD_DPAD_DOWN;
    }
    else if (elapsed_time_ms.count() < intervals_ms[7]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[8]) {
        rep.wButtons = XUSB_GAMEPAD_A;
    }
    else {
        rep = {};
        is_in_checkpoint_reset = false;
        is_checkpoint_started_ = false;
    }
    */
    if (elapsed_time_ms.count() < intervals_ms[0]) {
        rep.wButtons = XUSB_GAMEPAD_START;
    }
    else if (elapsed_time_ms.count() < intervals_ms[1]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[2]) {
        rep.wButtons = XUSB_GAMEPAD_DPAD_DOWN;
    }
    else if (elapsed_time_ms.count() < intervals_ms[3]) {
        rep = {};
    }
    else if (elapsed_time_ms.count() < intervals_ms[3] + last_keydown_time_s_ * 1000.) {
        rep.wButtons = XUSB_GAMEPAD_A;
    }
    else {
        rep = {};
        is_in_checkpoint_reset = false;
        is_checkpoint_started_ = false;
    }

    return rep;
}

void MenuButtonControl::ProcessINI() {
    ini_parser_.ParseConfig("Duration", "cmd_input_interval", cmd_input_interval_s_);
    ini_parser_.ParseConfig("Duration", "last_keydown_time", last_keydown_time_s_);
}




